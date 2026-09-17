#include "OwnCharacterStateDTO.h"

namespace Engine {

OwnCharacterStateDTO::OwnCharacterStateDTO() :
    _idCharacter( 0 ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ),
    _orientation( EntityOrientationEnum::SOUTH ) {
}

OwnCharacterStateDTO::~OwnCharacterStateDTO() = default;

OwnCharacterStateDTO OwnCharacterStateDTO::fromJson( const Json::Value& json ) {
    OwnCharacterStateDTO dto;

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

Json::Value OwnCharacterStateDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = "own_character_state";
    json[ "idCharacter" ] = _idCharacter;
    json[ "x" ] = _x;
    json[ "y" ] = _y;
    json[ "z" ] = _z;
    json[ "orientation" ] = static_cast<int>( _orientation );

    return json;
}

int OwnCharacterStateDTO::idCharacter() const {
    return _idCharacter;
}

void OwnCharacterStateDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

int OwnCharacterStateDTO::x() const {
    return _x;
}

void OwnCharacterStateDTO::setX( int x ) {
    _x = x;
}

int OwnCharacterStateDTO::y() const {
    return _y;
}

void OwnCharacterStateDTO::setY( int y ) {
    _y = y;
}

int OwnCharacterStateDTO::z() const {
    return _z;
}

void OwnCharacterStateDTO::setZ( int z ) {
    _z = z;
}

EntityOrientationEnum OwnCharacterStateDTO::orientation() const {
    return _orientation;
}

void OwnCharacterStateDTO::setOrientation( EntityOrientationEnum orientation ) {
    _orientation = orientation;
}

} // namespace Engine
