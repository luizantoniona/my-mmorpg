#include "CharacterEventAttackStartDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

CharacterEventAttackStartDTO::CharacterEventAttackStartDTO() :
    _castSeconds( 0.0 ),
    _idCharacter( 0 ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

CharacterEventAttackStartDTO::~CharacterEventAttackStartDTO() = default;

CharacterEventAttackStartDTO CharacterEventAttackStartDTO::fromJson( const Json::Value& json ) {
    CharacterEventAttackStartDTO dto;

    if ( json.isMember( "idCharacter" ) && json[ "idCharacter" ].isInt() ) {
        dto._idCharacter = json[ "idCharacter" ].asInt();
    }

    if ( json.isMember( "x" ) && json[ "x" ].isInt() ) {
        dto._x = json[ "x" ].asInt();
    }

    if ( json.isMember( "y" ) && json[ "y" ].isInt() ) {
        dto._y = json[ "y" ].asInt();
    }

    if ( json.isMember( "z" ) && json[ "z" ].isInt() ) {
        dto._z = json[ "z" ].asInt();
    }

    if ( json.isMember( "castSeconds" ) && json[ "castSeconds" ].isNumeric() ) {
        dto._castSeconds = json[ "castSeconds" ].asDouble();
    }

    return dto;
}

Json::Value CharacterEventAttackStartDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::CHARACTER_EVENT_ATTACK_START );
    json[ "idCharacter" ] = _idCharacter;
    json[ "x" ] = _x;
    json[ "y" ] = _y;
    json[ "z" ] = _z;
    json[ "castSeconds" ] = _castSeconds;

    return json;
}

int CharacterEventAttackStartDTO::idCharacter() const {
    return _idCharacter;
}

void CharacterEventAttackStartDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

int CharacterEventAttackStartDTO::x() const {
    return _x;
}

void CharacterEventAttackStartDTO::setX( int x ) {
    _x = x;
}

int CharacterEventAttackStartDTO::y() const {
    return _y;
}

void CharacterEventAttackStartDTO::setY( int y ) {
    _y = y;
}

int CharacterEventAttackStartDTO::z() const {
    return _z;
}

void CharacterEventAttackStartDTO::setZ( int z ) {
    _z = z;
}

double CharacterEventAttackStartDTO::castSeconds() const {
    return _castSeconds;
}

void CharacterEventAttackStartDTO::setCastSeconds( double castSeconds ) {
    _castSeconds = castSeconds;
}

} // namespace Engine
