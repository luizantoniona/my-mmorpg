#ifndef NETWORKMESSAGETYPEHELPER_H
#define NETWORKMESSAGETYPEHELPER_H

#include <string>

#include <json/json.h>

#include <MMORPGEngine/Network/WebSocket/NetworkMessageTypeEnum.h>

namespace Engine {

class NetworkMessageTypeHelper {
public:
    static std::string toString( NetworkMessageType type );
    static NetworkMessageType fromString( const std::string& value );
    static NetworkMessageType fromMessage( const Json::Value& json );
};

} // namespace Engine

#endif // NETWORKMESSAGETYPEHELPER_H
