#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <string>

#include <drogon/WebSocketController.h>
#include <json/json.h>

#include <MMORPGServer/Server/Runtime/WorldRuntime.h>

namespace Server {

class MessageReceiver {
public:
    MessageReceiver();

    void receive( const drogon::WebSocketConnectionPtr& connection, int idCharacter, const std::string& message );

private:
    void receiveMove( const drogon::WebSocketConnectionPtr& connection, int idCharacter, const Json::Value& messageJson );

private:
    WorldRuntime* _worldRuntime;
};

} // namespace Server

#endif // MESSAGERECEIVER_H
