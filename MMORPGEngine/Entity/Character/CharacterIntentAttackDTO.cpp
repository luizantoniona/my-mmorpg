#include "CharacterIntentAttackDTO.h"

#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeHelper.h>

namespace Engine {

CharacterIntentAttackDTO::CharacterIntentAttackDTO() :
    _dx( 0 ),
    _dy( 0 ) {
}

CharacterIntentAttackDTO::~CharacterIntentAttackDTO() = default;

CharacterIntentAttackDTO CharacterIntentAttackDTO::fromJson( const Json::Value& json ) {
    CharacterIntentAttackDTO dto;

    if ( json.isMember( "dx" ) && json[ "dx" ].isInt() ) {
        dto._dx = json[ "dx" ].asInt();
    }

    if ( json.isMember( "dy" ) && json[ "dy" ].isInt() ) {
        dto._dy = json[ "dy" ].asInt();
    }

    return dto;
}

Json::Value CharacterIntentAttackDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ClientMessageTypeHelper::toString( ClientMessageType::CHARACTER_INTENT_ATTACK );
    json[ "dx" ] = _dx;
    json[ "dy" ] = _dy;

    return json;
}

int CharacterIntentAttackDTO::dx() const {
    return _dx;
}

void CharacterIntentAttackDTO::setDx( int dx ) {
    _dx = dx;
}

int CharacterIntentAttackDTO::dy() const {
    return _dy;
}

void CharacterIntentAttackDTO::setDy( int dy ) {
    _dy = dy;
}

} // namespace Engine
