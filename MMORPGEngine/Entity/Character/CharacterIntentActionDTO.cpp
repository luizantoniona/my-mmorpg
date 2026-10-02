#include "CharacterIntentActionDTO.h"

#include <MMORPGEngine/Entity/CombatActionHelper.h>
#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeHelper.h>

namespace Engine {

CharacterIntentActionDTO::CharacterIntentActionDTO() :
    _action( std::nullopt ),
    _dx( 0 ),
    _dy( 0 ) {
}

CharacterIntentActionDTO::~CharacterIntentActionDTO() = default;

CharacterIntentActionDTO CharacterIntentActionDTO::fromJson( const Json::Value& json ) {
    CharacterIntentActionDTO dto;

    if ( json.isMember( "action" ) && json[ "action" ].isString() ) {
        dto._action = CombatActionHelper::fromString( json[ "action" ].asString() );
    }

    if ( json.isMember( "dx" ) && json[ "dx" ].isInt() ) {
        dto._dx = json[ "dx" ].asInt();
    }

    if ( json.isMember( "dy" ) && json[ "dy" ].isInt() ) {
        dto._dy = json[ "dy" ].asInt();
    }

    return dto;
}

Json::Value CharacterIntentActionDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ClientMessageTypeHelper::toString( ClientMessageType::CHARACTER_INTENT_ACTION );

    if ( _action ) {
        json[ "action" ] = CombatActionHelper::toString( *_action );
    }

    json[ "dx" ] = _dx;
    json[ "dy" ] = _dy;

    return json;
}

const std::optional<CombatActionEnum>& CharacterIntentActionDTO::action() const {
    return _action;
}

void CharacterIntentActionDTO::setAction( CombatActionEnum action ) {
    _action = action;
}

int CharacterIntentActionDTO::dx() const {
    return _dx;
}

void CharacterIntentActionDTO::setDx( int dx ) {
    _dx = dx;
}

int CharacterIntentActionDTO::dy() const {
    return _dy;
}

void CharacterIntentActionDTO::setDy( int dy ) {
    _dy = dy;
}

} // namespace Engine
