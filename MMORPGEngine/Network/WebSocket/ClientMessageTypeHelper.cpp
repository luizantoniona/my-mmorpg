#include "ClientMessageTypeHelper.h"

namespace Engine {

std::string ClientMessageTypeHelper::toString( ClientMessageType type ) {
    switch ( type ) {
    case ClientMessageType::CHARACTER_MOVE:
        return "CHARACTER_MOVE";
    case ClientMessageType::UNKNOWN:
        return "UNKNOWN";
    }

    return "UNKNOWN";
}

ClientMessageType ClientMessageTypeHelper::fromString( const std::string& value ) {
    if ( value == "CHARACTER_MOVE" ) {
        return ClientMessageType::CHARACTER_MOVE;
    }

    return ClientMessageType::UNKNOWN;
}

ClientMessageType ClientMessageTypeHelper::fromMessage( const Json::Value& json ) {
    if ( !json.isObject() || !json.isMember( "type" ) ) {
        return ClientMessageType::UNKNOWN;
    }

    return fromString( json[ "type" ].asString() );
}

} // namespace Engine
