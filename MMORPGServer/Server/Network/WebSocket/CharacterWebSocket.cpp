#include "CharacterWebSocket.h"

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/Entity/EntityStateDTO.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>
#include <MMORPGServer/Server/Network/NetworkServer.h>
#include <MMORPGServer/Server/Network/WebSocket/CharacterConnectionContext.h>
#include <MMORPGServer/Server/Repository/CharacterPositionRepository.h>
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
    const std::vector<int> floors = world->floors();

    Engine::EntityPositionModel position;
    auto persistedPosition = CharacterPositionRepository().find( idCharacter );

    if ( persistedPosition ) {
        position = persistedPosition->position();
        qInfo() << "[WebSocket] Loaded persisted position [CHARACTER]" << idCharacter << "[X]" << position.x() << "[Y]" << position.y() << "[Z]" << position.z();

    } else {
        position.setX( static_cast<int>( world->width() / 2 ) );
        position.setY( static_cast<int>( world->height() / 2 ) );
        position.setZ( floors.empty() ? 0 : floors.front() );
        qInfo() << "[WebSocket] No persisted position, using spawn point [CHARACTER]" << idCharacter << "[X]" << position.x() << "[Y]" << position.y() << "[Z]" << position.z();
    }

    character->setPosition( position );
    worldManager.addCharacter( std::move( character ) );

    connection->setContext( std::make_shared<CharacterConnectionContext>( sessionId, idCharacter ) );

    Engine::EntityStateDTO state;
    state.setIdCharacter( idCharacter );
    state.setX( position.x() );
    state.setY( position.y() );
    state.setZ( position.z() );
    state.setWorldName( world->name().toStdString() );

    connection->send( Engine::JsonHelper::writeJsonString( state.toJson() ) );

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
        Engine::CharacterPositionModel characterPosition;
        characterPosition.setIdCharacter( character->idCharacter() );
        characterPosition.setPosition( character->position() );

        qInfo() << "[WebSocket] Saving position on disconnect [CHARACTER]" << character->idCharacter();

        if ( !CharacterPositionRepository().save( characterPosition ) ) {
            qWarning() << "[WebSocket] Failed to save position [CHARACTER]" << character->idCharacter();
        }
    }

    worldManager.removeCharacter( contextPtr->idCharacter() );

    qInfo() << "[WebSocket] Character left world [CHARACTER]" << contextPtr->idCharacter();
}

} // namespace Server
