#include "CharacterMoveDTO.h"

#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeHelper.h>

namespace Engine {

CharacterMoveDTO::CharacterMoveDTO() :
    _dx( 0 ),
    _dy( 0 ) {
}

CharacterMoveDTO::~CharacterMoveDTO() = default;

CharacterMoveDTO CharacterMoveDTO::fromJson( const Json::Value& json ) {
    CharacterMoveDTO dto;

    if ( json.isMember( "dx" ) && json[ "dx" ].isInt() ) {
        dto._dx = json[ "dx" ].asInt();
    }

    if ( json.isMember( "dy" ) && json[ "dy" ].isInt() ) {
        dto._dy = json[ "dy" ].asInt();
    }

    return dto;
}

Json::Value CharacterMoveDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ClientMessageTypeHelper::toString( ClientMessageType::CHARACTER_MOVE );
    json[ "dx" ] = _dx;
    json[ "dy" ] = _dy;

    return json;
}

int CharacterMoveDTO::dx() const {
    return _dx;
}

void CharacterMoveDTO::setDx( int dx ) {
    _dx = dx;
}

int CharacterMoveDTO::dy() const {
    return _dy;
}

void CharacterMoveDTO::setDy( int dy ) {
    _dy = dy;
}

} // namespace Engine
