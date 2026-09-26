#include "CharacterAttackDTO.h"

#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeHelper.h>

namespace Engine {

CharacterAttackDTO::CharacterAttackDTO() :
    _dx( 0 ),
    _dy( 0 ) {
}

CharacterAttackDTO::~CharacterAttackDTO() = default;

CharacterAttackDTO CharacterAttackDTO::fromJson( const Json::Value& json ) {
    CharacterAttackDTO dto;

    if ( json.isMember( "dx" ) && json[ "dx" ].isInt() ) {
        dto._dx = json[ "dx" ].asInt();
    }

    if ( json.isMember( "dy" ) && json[ "dy" ].isInt() ) {
        dto._dy = json[ "dy" ].asInt();
    }

    return dto;
}

Json::Value CharacterAttackDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ClientMessageTypeHelper::toString( ClientMessageType::CHARACTER_ATTACK );
    json[ "dx" ] = _dx;
    json[ "dy" ] = _dy;

    return json;
}

int CharacterAttackDTO::dx() const {
    return _dx;
}

void CharacterAttackDTO::setDx( int dx ) {
    _dx = dx;
}

int CharacterAttackDTO::dy() const {
    return _dy;
}

void CharacterAttackDTO::setDy( int dy ) {
    _dy = dy;
}

} // namespace Engine
