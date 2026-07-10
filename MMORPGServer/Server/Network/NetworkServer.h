#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

#include "NetworkSession.h"

namespace Server {

class NetworkServer {
public:
    NetworkServer();
    ~NetworkServer();

    std::string createSession( const int idAccount, const std::string& username );

    bool deleteSession( const std::string& uuid );

    bool clearSessions();

    std::optional<NetworkSession> getSession( const std::string& uuid ) const;

private:
    mutable std::mutex _sessionMutex;
    std::unordered_map<std::string, NetworkSession> _sessions;
};

} // namespace Server

#endif // NETWORKSERVER_H
