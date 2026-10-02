#include "CharacterEventAttackDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

CharacterEventAttackDTO::CharacterEventAttackDTO() :
    _idCharacter( 0 ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

CharacterEventAttackDTO::~CharacterEventAttackDTO() = default;

CharacterEventAttackDTO CharacterEventAttackDTO::fromJson( const Json::Value& json ) {
    CharacterEventAttackDTO dto;

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

    return dto;
}

Json::Value CharacterEventAttackDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::CHARACTER_EVENT_ATTACK );
    json[ "idCharacter" ] = _idCharacter;
    json[ "x" ] = _x;
    json[ "y" ] = _y;
    json[ "z" ] = _z;

    return json;
}

int CharacterEventAttackDTO::idCharacter() const {
    return _idCharacter;
}

void CharacterEventAttackDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

int CharacterEventAttackDTO::x() const {
    return _x;
}

void CharacterEventAttackDTO::setX( int x ) {
    _x = x;
}

int CharacterEventAttackDTO::y() const {
    return _y;
}

void CharacterEventAttackDTO::setY( int y ) {
    _y = y;
}

int CharacterEventAttackDTO::z() const {
    return _z;
}

void CharacterEventAttackDTO::setZ( int z ) {
    _z = z;
}

} // namespace Engine
