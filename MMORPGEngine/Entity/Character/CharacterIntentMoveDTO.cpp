#include "CharacterIntentMoveDTO.h"

#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeHelper.h>

namespace Engine {

CharacterIntentMoveDTO::CharacterIntentMoveDTO() :
    _dx( 0 ),
    _dy( 0 ) {
}

CharacterIntentMoveDTO::~CharacterIntentMoveDTO() = default;

CharacterIntentMoveDTO CharacterIntentMoveDTO::fromJson( const Json::Value& json ) {
    CharacterIntentMoveDTO dto;

    if ( json.isMember( "dx" ) && json[ "dx" ].isInt() ) {
        dto._dx = json[ "dx" ].asInt();
    }

    if ( json.isMember( "dy" ) && json[ "dy" ].isInt() ) {
        dto._dy = json[ "dy" ].asInt();
    }

    return dto;
}

Json::Value CharacterIntentMoveDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ClientMessageTypeHelper::toString( ClientMessageType::CHARACTER_INTENT_MOVE );
    json[ "dx" ] = _dx;
    json[ "dy" ] = _dy;

    return json;
}

int CharacterIntentMoveDTO::dx() const {
    return _dx;
}

void CharacterIntentMoveDTO::setDx( int dx ) {
    _dx = dx;
}

int CharacterIntentMoveDTO::dy() const {
    return _dy;
}

void CharacterIntentMoveDTO::setDy( int dy ) {
    _dy = dy;
}

} // namespace Engine
