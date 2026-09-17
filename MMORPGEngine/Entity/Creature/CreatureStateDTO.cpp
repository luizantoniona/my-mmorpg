#include "CreatureStateDTO.h"

namespace Engine {

CreatureStateDTO::CreatureStateDTO() :
    _idCreature( 0 ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ),
    _orientation( EntityOrientationEnum::SOUTH ) {
}

CreatureStateDTO::~CreatureStateDTO() = default;

CreatureStateDTO CreatureStateDTO::fromJson( const Json::Value& json ) {
    CreatureStateDTO dto;

    if ( json.isMember( "idCreature" ) && json[ "idCreature" ].isInt() ) {
        dto._idCreature = json[ "idCreature" ].asInt();
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

Json::Value CreatureStateDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = "creature_state";
    json[ "idCreature" ] = _idCreature;
    json[ "x" ] = _x;
    json[ "y" ] = _y;
    json[ "z" ] = _z;
    json[ "orientation" ] = static_cast<int>( _orientation );

    return json;
}

int CreatureStateDTO::idCreature() const {
    return _idCreature;
}

void CreatureStateDTO::setIdCreature( int idCreature ) {
    _idCreature = idCreature;
}

int CreatureStateDTO::x() const {
    return _x;
}

void CreatureStateDTO::setX( int x ) {
    _x = x;
}

int CreatureStateDTO::y() const {
    return _y;
}

void CreatureStateDTO::setY( int y ) {
    _y = y;
}

int CreatureStateDTO::z() const {
    return _z;
}

void CreatureStateDTO::setZ( int z ) {
    _z = z;
}

EntityOrientationEnum CreatureStateDTO::orientation() const {
    return _orientation;
}

void CreatureStateDTO::setOrientation( EntityOrientationEnum orientation ) {
    _orientation = orientation;
}

} // namespace Engine
