#include "CharacterStateDTO.h"

namespace Engine {

CharacterStateDTO::CharacterStateDTO() :
    _idCharacter( 0 ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ),
    _orientation( EntityOrientationEnum::SOUTH ) {
}

CharacterStateDTO::~CharacterStateDTO() = default;

CharacterStateDTO CharacterStateDTO::fromJson( const Json::Value& json ) {
    CharacterStateDTO dto;

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

    if ( json.isMember( "orientation" ) && json[ "orientation" ].isInt() ) {
        dto._orientation = static_cast<EntityOrientationEnum>( json[ "orientation" ].asInt() );
    }

    return dto;
}

Json::Value CharacterStateDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = "character_state";
    json[ "idCharacter" ] = _idCharacter;
    json[ "x" ] = _x;
    json[ "y" ] = _y;
    json[ "z" ] = _z;
    json[ "orientation" ] = static_cast<int>( _orientation );

    return json;
}

int CharacterStateDTO::idCharacter() const {
    return _idCharacter;
}

void CharacterStateDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

int CharacterStateDTO::x() const {
    return _x;
}

void CharacterStateDTO::setX( int x ) {
    _x = x;
}

int CharacterStateDTO::y() const {
    return _y;
}

void CharacterStateDTO::setY( int y ) {
    _y = y;
}

int CharacterStateDTO::z() const {
    return _z;
}

void CharacterStateDTO::setZ( int z ) {
    _z = z;
}

EntityOrientationEnum CharacterStateDTO::orientation() const {
    return _orientation;
}

void CharacterStateDTO::setOrientation( EntityOrientationEnum orientation ) {
    _orientation = orientation;
}

} // namespace Engine
