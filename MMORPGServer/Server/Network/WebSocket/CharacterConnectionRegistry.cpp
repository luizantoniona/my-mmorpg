#include "CharacterConnectionRegistry.h"

namespace Server {

CharacterConnectionRegistry::CharacterConnectionRegistry() {
}

void CharacterConnectionRegistry::registerConnection( int idCharacter, const drogon::WebSocketConnectionPtr& connection ) {
    std::lock_guard<std::mutex> lock( _mutex );

    _connections[ idCharacter ] = connection;
}

void CharacterConnectionRegistry::unregisterConnection( int idCharacter ) {
    std::lock_guard<std::mutex> lock( _mutex );

    _connections.erase( idCharacter );
}

drogon::WebSocketConnectionPtr CharacterConnectionRegistry::connection( int idCharacter ) {
    std::lock_guard<std::mutex> lock( _mutex );

    auto it = _connections.find( idCharacter );
    return it != _connections.end() ? it->second : nullptr;
}

} // namespace Server
