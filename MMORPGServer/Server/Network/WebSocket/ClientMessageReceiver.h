#ifndef CLIENTMESSAGERECEIVER_H
#define CLIENTMESSAGERECEIVER_H

#include <string>

#include <drogon/WebSocketController.h>
#include <json/json.h>

#include <MMORPGEngine/Entity/Character/CharacterIntentActionDTO.h>
#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace Server {

class ClientMessageReceiver {
public:
    ClientMessageReceiver();

    void receive( const drogon::WebSocketConnectionPtr& connection, int idCharacter, const std::string& message );

private:
    void receiveMove( const drogon::WebSocketConnectionPtr& connection, int idCharacter, const Json::Value& messageJson );
    void receiveAction( int idCharacter, const Json::Value& messageJson );
    void receiveAttack( int idCharacter, const Engine::CharacterIntentActionDTO& input );

private:
    WorldRuntime* _worldRuntime;
};

} // namespace Server

#endif // CLIENTMESSAGERECEIVER_H
