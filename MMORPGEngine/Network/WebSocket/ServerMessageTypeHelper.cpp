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
    case ServerMessageType::CHARACTER_EVENT_ATTACK_START:
        return "CHARACTER_EVENT_ATTACK_START";
    case ServerMessageType::CHARACTER_EVENT_ATTACK:
        return "CHARACTER_EVENT_ATTACK";
    case ServerMessageType::CHARACTER_EVENT_LEAVE:
        return "CHARACTER_EVENT_LEAVE";
    case ServerMessageType::CREATURE:
        return "CREATURE";
    case ServerMessageType::CREATURE_EVENT_ATTACK_START:
        return "CREATURE_EVENT_ATTACK_START";
    case ServerMessageType::CREATURE_EVENT_ATTACK:
        return "CREATURE_EVENT_ATTACK";
    case ServerMessageType::CREATURE_EVENT_LEAVE:
        return "CREATURE_EVENT_LEAVE";
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
    if ( value == "CHARACTER_EVENT_ATTACK_START" ) {
        return ServerMessageType::CHARACTER_EVENT_ATTACK_START;
    }
    if ( value == "CHARACTER_EVENT_ATTACK" ) {
        return ServerMessageType::CHARACTER_EVENT_ATTACK;
    }
    if ( value == "CHARACTER_EVENT_LEAVE" ) {
        return ServerMessageType::CHARACTER_EVENT_LEAVE;
    }
    if ( value == "CREATURE" ) {
        return ServerMessageType::CREATURE;
    }
    if ( value == "CREATURE_EVENT_ATTACK_START" ) {
        return ServerMessageType::CREATURE_EVENT_ATTACK_START;
    }
    if ( value == "CREATURE_EVENT_ATTACK" ) {
        return ServerMessageType::CREATURE_EVENT_ATTACK;
    }
    if ( value == "CREATURE_EVENT_LEAVE" ) {
        return ServerMessageType::CREATURE_EVENT_LEAVE;
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
