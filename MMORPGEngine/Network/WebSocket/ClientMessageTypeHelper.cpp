#include "ClientMessageTypeHelper.h"

namespace Engine {

std::string ClientMessageTypeHelper::toString( ClientMessageType type ) {
    switch ( type ) {
    case ClientMessageType::CHARACTER_INTENT_MOVE:
        return "CHARACTER_INTENT_MOVE";
    case ClientMessageType::CHARACTER_INTENT_ATTACK:
        return "CHARACTER_INTENT_ATTACK";
    case ClientMessageType::UNKNOWN:
        return "UNKNOWN";
    }

    return "UNKNOWN";
}

ClientMessageType ClientMessageTypeHelper::fromString( const std::string& value ) {
    if ( value == "CHARACTER_INTENT_MOVE" ) {
        return ClientMessageType::CHARACTER_INTENT_MOVE;
    }

    if ( value == "CHARACTER_INTENT_ATTACK" ) {
        return ClientMessageType::CHARACTER_INTENT_ATTACK;
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
