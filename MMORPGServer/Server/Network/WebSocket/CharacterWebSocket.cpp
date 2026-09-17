#include "CharacterWebSocket.h"

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>
#include <MMORPGServer/Server/Network/NetworkServer.h>
#include <MMORPGServer/Server/Network/WebSocket/CharacterConnectionContext.h>
#include <MMORPGServer/Server/Network/WebSocket/CharacterConnectionRegistry.h>
#include <MMORPGServer/Server/Repository/Character/CharacterRepository.h>

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

    connection->setContext( std::make_shared<CharacterConnectionContext>( sessionId, idCharacter ) );

    Engine::Singleton<CharacterConnectionRegistry>::instance().registerConnection( idCharacter, connection );

    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    worldRuntime.addCharacter( std::move( character ) );

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
