#include "ClientMessageTypeHelper.h"

namespace Engine {

std::string ClientMessageTypeHelper::toString( ClientMessageType type ) {
    switch ( type ) {
    case ClientMessageType::CHARACTER_INTENT_MOVE:
        return "CHARACTER_INTENT_MOVE";
    case ClientMessageType::CHARACTER_INTENT_ACTION:
        return "CHARACTER_INTENT_ACTION";
    case ClientMessageType::UNKNOWN:
        return "UNKNOWN";
    }

    return "UNKNOWN";
}

ClientMessageType ClientMessageTypeHelper::fromString( const std::string& value ) {
    if ( value == "CHARACTER_INTENT_MOVE" ) {
        return ClientMessageType::CHARACTER_INTENT_MOVE;
    }

    if ( value == "CHARACTER_INTENT_ACTION" ) {
        return ClientMessageType::CHARACTER_INTENT_ACTION;
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
