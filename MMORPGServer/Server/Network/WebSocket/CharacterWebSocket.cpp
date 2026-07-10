#include "CharacterWebSocket.h"

#include <MMORPGEngine/Commons/Singleton.h>
#include <Manager/WorldManager.h>
#include <Network/NetworkServer.h>

namespace Server {

void CharacterWebSocket::handleNewMessage( const drogon::WebSocketConnectionPtr& connection, std::string&& message, const drogon::WebSocketMessageType& type ) {
    auto sessionIdPtr = connection->getContext<std::string>();

    if ( !sessionIdPtr ) {
        connection->send( R"({"error":"missing_context"})" );
        connection->shutdown();
        return;
    }

    _receiver.receive( *sessionIdPtr, message );
}

void CharacterWebSocket::handleNewConnection( const drogon::HttpRequestPtr& request, const drogon::WebSocketConnectionPtr& connection ) {
    std::cout << "[WebSocket] New connection: " << connection->peerAddr().toIp() << std::endl;

    std::string sessionId;
    const auto token = request->getHeader( "Authorization" );
    const std::string prefix = "X-Session ";

    if ( !token.empty() && token.rfind( prefix, 0 ) == 0 ) {
        sessionId = token.substr( prefix.length() );

    } else {
        sessionId = request->getParameter( "sid" );
    }

    if ( sessionId.empty() ) {
        connection->send( R"({"error":"missing_session"})" );
        connection->shutdown();
        return;
    }

    const auto characterParam = request->getParameter( "character" );
    if ( characterParam.empty() ) {
        connection->send( R"({"error":"missing_character"})" );
        connection->shutdown();
        return;
    }

    int idCharacter = std::stoi( characterParam );

    auto& server = Engine::Singleton<NetworkServer>::instance();
    auto session = server.getSession( sessionId );
    if ( !session ) {
        connection->send( R"({"error":"invalid_session"})" );
        connection->shutdown();
        return;
    }

    connection->setContext( std::make_shared<std::string>( sessionId ) );

    // if ( !Singleton<WorldManager>::instance().addCharacter( sessionId, session->idAccount(), idCharacter, connection ) ) {
    //     connection->send( R"({"error":"invalid_character"})" );
    //     connection->shutdown();
    //     return;
    // }
}

void CharacterWebSocket::handleConnectionClosed( const drogon::WebSocketConnectionPtr& connection ) {
    std::cout << "[WebSocket] Connection closed: " << connection->peerAddr().toIp() << std::endl;

    auto uuidPtr = connection->getContext<std::string>();

    if ( !uuidPtr ) {
        return;
    }

    // Singleton<WorldManager>::instance().removeCharacter( *uuidPtr );
}

} // namespace Server
