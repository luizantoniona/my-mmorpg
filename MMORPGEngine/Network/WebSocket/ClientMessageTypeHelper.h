#ifndef CLIENTMESSAGETYPEHELPER_H
#define CLIENTMESSAGETYPEHELPER_H

#include <string>

#include <json/json.h>

#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeEnum.h>

namespace Engine {

class ClientMessageTypeHelper {
public:
    static std::string toString( ClientMessageType type );
    static ClientMessageType fromString( const std::string& value );
    static ClientMessageType fromMessage( const Json::Value& json );
};

} // namespace Engine

#endif // CLIENTMESSAGETYPEHELPER_H
