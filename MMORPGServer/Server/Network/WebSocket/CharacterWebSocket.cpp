#include "CharacterWebSocket.h"

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/EntityOrientationEnum.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/Entity/EntityStateDTO.h>
#include <MMORPGEngine/Entity/EntityVitalsDTO.h>
#include <MMORPGEngine/Entity/EntityVitalsModel.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>
#include <MMORPGServer/Server/Network/NetworkServer.h>
#include <MMORPGServer/Server/Network/WebSocket/CharacterConnectionContext.h>
#include <MMORPGServer/Server/Network/WebSocket/CharacterConnectionRegistry.h>
#include <MMORPGServer/Server/Repository/CharacterRepository.h>

namespace Server {

void CharacterWebSocket::handleNewMessage( const drogon::WebSocketConnectionPtr& connection, std::string&& message, const drogon::WebSocketMessageType& type ) {
    auto contextPtr = connection->getContext<CharacterConnectionContext>();

    if ( !contextPtr ) {
        connection->send( R"({"error":"missing_context"})" );
        connection->shutdown();
        return;
    }

    _receiver.receive( connection, contextPtr->idCharacter(), message );
}

void CharacterWebSocket::handleNewConnection( const drogon::HttpRequestPtr& request, const drogon::WebSocketConnectionPtr& connection ) {
    qInfo() << "[WebSocket] New connection: " << connection->peerAddr().toIp();

    std::string sessionId;
    const auto token = request->getHeader( "Authorization" );
    const std::string prefix = "X-Session ";

    if ( !token.empty() && token.rfind( prefix, 0 ) == 0 ) {
        sessionId = token.substr( prefix.length() );

    } else {
        sessionId = request->getParameter( "sid" );
    }

    if ( sessionId.empty() ) {
        qWarning() << "[WebSocket] Rejected connection: missing session";
        connection->send( R"({"error":"missing_session"})" );
        connection->shutdown();
        return;
    }

    const auto characterParam = request->getParameter( "character" );
    if ( characterParam.empty() ) {
        qWarning() << "[WebSocket] Rejected connection: missing character";
        connection->send( R"({"error":"missing_character"})" );
        connection->shutdown();
        return;
    }

    int idCharacter = std::stoi( characterParam );

    auto& server = Engine::Singleton<NetworkServer>::instance();
    auto session = server.getSession( sessionId );
    if ( !session ) {
        qWarning() << "[WebSocket] Rejected connection: invalid session [CHARACTER]" << idCharacter;
        connection->send( R"({"error":"invalid_session"})" );
        connection->shutdown();
        return;
    }

    auto character = CharacterRepository().findByIdAccountAndIdCharacter( session->idAccount(), idCharacter );
    if ( !character ) {
        qWarning() << "[WebSocket] Rejected connection: invalid character [ACCOUNT]" << session->idAccount() << "[CHARACTER]" << idCharacter;
        connection->send( R"({"error":"invalid_character"})" );
        connection->shutdown();
        return;
    }

    qInfo() << "[WebSocket] Character entering world [ACCOUNT]" << session->idAccount() << "[CHARACTER]" << idCharacter;

    auto& worldManager = Engine::Singleton<WorldManager>::instance();
    const Engine::WorldModel* world = worldManager.world();

    const Engine::EntityPositionModel position = character->position();
    const Engine::EntityOrientationEnum orientation = character->orientation().direction();
    const Engine::EntityVitalsModel vitals = character->vitals();
    qInfo() << "[WebSocket] Character position [CHARACTER]" << idCharacter << "[X]" << position.x() << "[Y]" << position.y() << "[Z]" << position.z();

    worldManager.addCharacter( std::move( character ) );

    connection->setContext( std::make_shared<CharacterConnectionContext>( sessionId, idCharacter ) );

    Engine::Singleton<CharacterConnectionRegistry>::instance().registerConnection( idCharacter, connection );

    Engine::EntityStateDTO state;
    state.setIdCharacter( idCharacter );
    state.setX( position.x() );
    state.setY( position.y() );
    state.setZ( position.z() );
    state.setOrientation( orientation );
    state.setWorldName( world->name().toStdString() );

    connection->send( Engine::JsonHelper::writeJsonString( state.toJson() ) );

    Engine::EntityVitalsDTO vitalsState;
    vitalsState.setIdCharacter( idCharacter );
    vitalsState.setHealth( vitals.health() );
    vitalsState.setMaxHealth( vitals.maxHealth() );
    vitalsState.setMana( vitals.mana() );
    vitalsState.setMaxMana( vitals.maxMana() );
    vitalsState.setStamina( vitals.stamina() );
    vitalsState.setMaxStamina( vitals.maxStamina() );

    connection->send( Engine::JsonHelper::writeJsonString( vitalsState.toJson() ) );

    for ( int nearbyIdCharacter : worldManager.charactersNear( idCharacter ) ) {
        Engine::CharacterModel* nearbyCharacter = worldManager.character( nearbyIdCharacter );
        if ( !nearbyCharacter ) {
            continue;
        }

        const Engine::EntityPositionModel& nearbyPosition = nearbyCharacter->position();
        const Engine::EntityVitalsModel& nearbyVitals = nearbyCharacter->vitals();

        Engine::EntityStateDTO nearbyState;
        nearbyState.setIdCharacter( nearbyIdCharacter );
        nearbyState.setX( nearbyPosition.x() );
        nearbyState.setY( nearbyPosition.y() );
        nearbyState.setZ( nearbyPosition.z() );
        nearbyState.setOrientation( nearbyCharacter->orientation().direction() );
        nearbyState.setWorldName( world->name().toStdString() );

        connection->send( Engine::JsonHelper::writeJsonString( nearbyState.toJson() ) );

        Engine::EntityVitalsDTO nearbyVitalsState;
        nearbyVitalsState.setIdCharacter( nearbyIdCharacter );
        nearbyVitalsState.setHealth( nearbyVitals.health() );
        nearbyVitalsState.setMaxHealth( nearbyVitals.maxHealth() );
        nearbyVitalsState.setMana( nearbyVitals.mana() );
        nearbyVitalsState.setMaxMana( nearbyVitals.maxMana() );
        nearbyVitalsState.setStamina( nearbyVitals.stamina() );
        nearbyVitalsState.setMaxStamina( nearbyVitals.maxStamina() );

        connection->send( Engine::JsonHelper::writeJsonString( nearbyVitalsState.toJson() ) );
    }

    qInfo() << "[WebSocket] Character entered world [CHARACTER]" << idCharacter;
}

void CharacterWebSocket::handleConnectionClosed( const drogon::WebSocketConnectionPtr& connection ) {
    qInfo() << "[WebSocket] Connection closed: " << connection->peerAddr().toIp();

    auto contextPtr = connection->getContext<CharacterConnectionContext>();

    if ( !contextPtr ) {
        return;
    }

    auto& worldManager = Engine::Singleton<WorldManager>::instance();
    Engine::CharacterModel* character = worldManager.character( contextPtr->idCharacter() );

    if ( character ) {
        qInfo() << "[WebSocket] Saving character on disconnect [CHARACTER]" << character->idCharacter();

        if ( !CharacterRepository().updateCharacter( *character ) ) {
            qWarning() << "[WebSocket] Failed to save character [CHARACTER]" << character->idCharacter();
        }
    }

    worldManager.removeCharacter( contextPtr->idCharacter() );

    Engine::Singleton<CharacterConnectionRegistry>::instance().unregisterConnection( contextPtr->idCharacter() );

    qInfo() << "[WebSocket] Character left world [CHARACTER]" << contextPtr->idCharacter();
}

} // namespace Server
