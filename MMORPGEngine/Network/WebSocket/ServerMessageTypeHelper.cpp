#include "ServerMessageTypeHelper.h"

namespace Engine {

std::string ServerMessageTypeHelper::toString( ServerMessageType type ) {
    switch ( type ) {
    case ServerMessageType::WORLD_BASIC:
        return "WORLD_BASIC";
    case ServerMessageType::OWN_CHARACTER:
        return "OWN_CHARACTER";
    case ServerMessageType::OWN_EQUIPMENT:
        return "OWN_EQUIPMENT";
    case ServerMessageType::OWN_INVENTORY:
        return "OWN_INVENTORY";
    case ServerMessageType::CHARACTER:
        return "CHARACTER";
    case ServerMessageType::CREATURE:
        return "CREATURE";
    case ServerMessageType::ENTITY_LEFT:
        return "ENTITY_LEFT";
    case ServerMessageType::CREATURE_LEFT:
        return "CREATURE_LEFT";
    case ServerMessageType::UNKNOWN:
        return "UNKNOWN";
    }

    return "UNKNOWN";
}

ServerMessageType ServerMessageTypeHelper::fromString( const std::string& value ) {
    if ( value == "WORLD_BASIC" ) {
        return ServerMessageType::WORLD_BASIC;
    }
    if ( value == "OWN_CHARACTER" ) {
        return ServerMessageType::OWN_CHARACTER;
    }
    if ( value == "OWN_EQUIPMENT" ) {
        return ServerMessageType::OWN_EQUIPMENT;
    }
    if ( value == "OWN_INVENTORY" ) {
        return ServerMessageType::OWN_INVENTORY;
    }
    if ( value == "CHARACTER" ) {
        return ServerMessageType::CHARACTER;
    }
    if ( value == "CREATURE" ) {
        return ServerMessageType::CREATURE;
    }
    if ( value == "ENTITY_LEFT" ) {
        return ServerMessageType::ENTITY_LEFT;
    }
    if ( value == "CREATURE_LEFT" ) {
        return ServerMessageType::CREATURE_LEFT;
    }

    return ServerMessageType::UNKNOWN;
}

ServerMessageType ServerMessageTypeHelper::fromMessage( const Json::Value& json ) {
    if ( !json.isObject() || !json.isMember( "type" ) ) {
        return ServerMessageType::UNKNOWN;
    }

    return fromString( json[ "type" ].asString() );
}

} // namespace Engine
