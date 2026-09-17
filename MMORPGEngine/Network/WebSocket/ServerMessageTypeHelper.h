#ifndef SERVERMESSAGETYPEHELPER_H
#define SERVERMESSAGETYPEHELPER_H

#include <string>

#include <json/json.h>

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeEnum.h>

namespace Engine {

class ServerMessageTypeHelper {
public:
    static std::string toString( ServerMessageType type );
    static ServerMessageType fromString( const std::string& value );
    static ServerMessageType fromMessage( const Json::Value& json );
};

} // namespace Engine

#endif // SERVERMESSAGETYPEHELPER_H
