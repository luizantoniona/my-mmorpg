#include "NetworkMessageTypeHelper.h"

namespace Engine {

std::string NetworkMessageTypeHelper::toString( NetworkMessageType type ) {
    switch ( type ) {
    case NetworkMessageType::MOVE:
        return "move";
    case NetworkMessageType::OWN_CHARACTER:
        return "own_character";
    case NetworkMessageType::CHARACTER:
        return "character";
    case NetworkMessageType::CREATURE_STATE:
        return "creature_state";
    case NetworkMessageType::ENTITY_LEFT:
        return "entity_left";
    case NetworkMessageType::UNKNOWN:
        return "unknown";
    }

    return "unknown";
}

NetworkMessageType NetworkMessageTypeHelper::fromString( const std::string& value ) {
    if ( value == "move" ) {
        return NetworkMessageType::MOVE;
    }
    if ( value == "own_character" ) {
        return NetworkMessageType::OWN_CHARACTER;
    }
    if ( value == "character" ) {
        return NetworkMessageType::CHARACTER;
    }
    if ( value == "creature_state" ) {
        return NetworkMessageType::CREATURE_STATE;
    }
    if ( value == "entity_left" ) {
        return NetworkMessageType::ENTITY_LEFT;
    }

    return NetworkMessageType::UNKNOWN;
}

NetworkMessageType NetworkMessageTypeHelper::fromMessage( const Json::Value& json ) {
    if ( !json.isObject() || !json.isMember( "type" ) ) {
        return NetworkMessageType::UNKNOWN;
    }

    return fromString( json[ "type" ].asString() );
}

} // namespace Engine
