#ifndef CHARACTEROBSERVER_H
#define CHARACTEROBSERVER_H

#include <drogon/WebSocketConnection.h>

#include <MMORPGServer/Server/Network/Observer/EntityObserver.h>

namespace Server {

class CharacterObserver : public EntityObserver {
public:
    explicit CharacterObserver( const drogon::WebSocketConnectionPtr& connection );
    ~CharacterObserver() override;

    void send( const std::string& message ) override;

    void shutdown() override;

private:
    drogon::WebSocketConnectionPtr _connection;
};

} // namespace Server

#endif // CHARACTEROBSERVER_H
