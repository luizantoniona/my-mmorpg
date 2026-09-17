#include "CharacterWebSocket.h"

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/Character/CharacterStateDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterVitalsDTO.h>
#include <MMORPGEngine/Entity/Character/OwnCharacterStateDTO.h>
#include <MMORPGEngine/Entity/Character/OwnCharacterVitalsDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureStateDTO.h>
#include <MMORPGEngine/Entity/EntityOrientationEnum.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/Entity/EntityVitalsModel.h>
#include <MMORPGEngine/World/WorldBasicDTO.h>
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

    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();
    const Engine::WorldModel* world = worldRuntime.world();

    const Engine::EntityPositionModel position = character->position();
    const Engine::EntityOrientationEnum orientation = character->orientation().direction();
    const Engine::EntityVitalsModel vitals = character->vitals();

    qInfo() << "[WebSocket] Character position [CHARACTER]" << idCharacter << "[X]" << position.x() << "[Y]" << position.y() << "[Z]" << position.z();

    worldRuntime.addCharacter( std::move( character ) );

    connection->setContext( std::make_shared<CharacterConnectionContext>( sessionId, idCharacter ) );

    Engine::Singleton<CharacterConnectionRegistry>::instance().registerConnection( idCharacter, connection );

    Engine::WorldBasicDTO worldBasic;
    worldBasic.setWorldName( world->name().toStdString() );

    connection->send( Engine::JsonHelper::writeJsonString( worldBasic.toJson() ) );

    Engine::OwnCharacterDTO state;
    state.setIdCharacter( idCharacter );
    state.setX( position.x() );
    state.setY( position.y() );
    state.setZ( position.z() );
    state.setOrientation( orientation );
    state.setHealth( vitals.health() );
    state.setMaxHealth( vitals.maxHealth() );
    state.setMana( vitals.mana() );
    state.setMaxMana( vitals.maxMana() );
    state.setStamina( vitals.stamina() );
    state.setMaxStamina( vitals.maxStamina() );

    connection->send( Engine::JsonHelper::writeJsonString( state.toJson() ) );

    for ( int nearbyIdCharacter : worldRuntime.charactersNear( idCharacter ) ) {
        Engine::CharacterModel* nearbyCharacter = worldRuntime.character( nearbyIdCharacter );
        if ( !nearbyCharacter ) {
            continue;
        }

        const Engine::EntityPositionModel& nearbyPosition = nearbyCharacter->position();
        const Engine::EntityVitalsModel& nearbyVitals = nearbyCharacter->vitals();

        Engine::CharacterDTO nearbyState;
        nearbyState.setIdCharacter( nearbyIdCharacter );
        nearbyState.setX( nearbyPosition.x() );
        nearbyState.setY( nearbyPosition.y() );
        nearbyState.setZ( nearbyPosition.z() );
        nearbyState.setOrientation( nearbyCharacter->orientation().direction() );
        nearbyState.setHealth( nearbyVitals.health() );
        nearbyState.setMaxHealth( nearbyVitals.maxHealth() );
        nearbyState.setMana( nearbyVitals.mana() );
        nearbyState.setMaxMana( nearbyVitals.maxMana() );
        nearbyState.setStamina( nearbyVitals.stamina() );
        nearbyState.setMaxStamina( nearbyVitals.maxStamina() );

        connection->send( Engine::JsonHelper::writeJsonString( nearbyState.toJson() ) );
    }

    // TODO: Filter by proximity once creatures move/spawn dynamically (Backlog "Monstros")
    for ( const Engine::CreatureModel& creature : worldRuntime.creatures() ) {
        const Engine::EntityPositionModel& creaturePosition = creature.position();

        Engine::CreatureStateDTO creatureState;
        creatureState.setIdCreature( creature.idCreature() );
        creatureState.setX( creaturePosition.x() );
        creatureState.setY( creaturePosition.y() );
        creatureState.setZ( creaturePosition.z() );
        creatureState.setOrientation( creature.orientation().direction() );

        connection->send( Engine::JsonHelper::writeJsonString( creatureState.toJson() ) );
    }

    qInfo() << "[WebSocket] Character entered world [CHARACTER]" << idCharacter;
}

void CharacterWebSocket::handleConnectionClosed( const drogon::WebSocketConnectionPtr& connection ) {
    qInfo() << "[WebSocket] Connection closed: " << connection->peerAddr().toIp();

    auto contextPtr = connection->getContext<CharacterConnectionContext>();

    if ( !contextPtr ) {
        return;
    }

    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();
    Engine::CharacterModel* character = worldRuntime.character( contextPtr->idCharacter() );

    if ( character ) {
        qInfo() << "[WebSocket] Saving character on disconnect [CHARACTER]" << character->idCharacter();

        if ( !CharacterRepository().updateCharacter( *character ) ) {
            qWarning() << "[WebSocket] Failed to save character [CHARACTER]" << character->idCharacter();
        }
    }

    worldRuntime.removeCharacter( contextPtr->idCharacter() );

    Engine::Singleton<CharacterConnectionRegistry>::instance().unregisterConnection( contextPtr->idCharacter() );

    qInfo() << "[WebSocket] Character left world [CHARACTER]" << contextPtr->idCharacter();
}

} // namespace Server
