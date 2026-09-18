#ifndef CHARACTERCONNECTIONREGISTRY_H
#define CHARACTERCONNECTIONREGISTRY_H

#include <mutex>
#include <unordered_map>

#include <drogon/WebSocketConnection.h>

namespace Server {

class CharacterConnectionRegistry {
public:
    CharacterConnectionRegistry();

    void registerConnection( int idCharacter, const drogon::WebSocketConnectionPtr& connection );
    void unregisterConnection( int idCharacter );

    drogon::WebSocketConnectionPtr connection( int idCharacter );

private:
    std::mutex _mutex;
    std::unordered_map<int, drogon::WebSocketConnectionPtr> _connections;
};

} // namespace Server

#endif // CHARACTERCONNECTIONREGISTRY_H
