#include "CharacterObserver.h"

namespace Server {

CharacterObserver::CharacterObserver( const drogon::WebSocketConnectionPtr& connection ) :
    _connection( connection ) {
}

CharacterObserver::~CharacterObserver() = default;

void CharacterObserver::send( const std::string& message ) {
    if ( !_connection || !_connection->connected() ) {
        return;
    }

    _connection->send( message );
}

void CharacterObserver::shutdown() {
    if ( !_connection ) {
        return;
    }

    _connection->shutdown();
}

} // namespace Server
