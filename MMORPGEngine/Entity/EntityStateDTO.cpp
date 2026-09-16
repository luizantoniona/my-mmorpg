#include "EntityStateDTO.h"

namespace Engine {

EntityStateDTO::EntityStateDTO() :
    _idCharacter( 0 ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ),
    _orientation( EntityOrientationEnum::SOUTH ),
    _worldName( "" ) {
}

EntityStateDTO::~EntityStateDTO() = default;

EntityStateDTO EntityStateDTO::fromJson( const Json::Value& json ) {
    EntityStateDTO dto;

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

    if ( json.isMember( "worldName" ) && json[ "worldName" ].isString() ) {
        dto._worldName = json[ "worldName" ].asString();
    }

    return dto;
}

Json::Value EntityStateDTO::toJson() const {
    Json::Value json;

    json[ "idCharacter" ] = _idCharacter;
    json[ "x" ] = _x;
    json[ "y" ] = _y;
    json[ "z" ] = _z;
    json[ "orientation" ] = static_cast<int>( _orientation );
    json[ "worldName" ] = _worldName;

    return json;
}

int EntityStateDTO::idCharacter() const {
    return _idCharacter;
}

void EntityStateDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

int EntityStateDTO::x() const {
    return _x;
}

void EntityStateDTO::setX( int x ) {
    _x = x;
}

int EntityStateDTO::y() const {
    return _y;
}

void EntityStateDTO::setY( int y ) {
    _y = y;
}

int EntityStateDTO::z() const {
    return _z;
}

void EntityStateDTO::setZ( int z ) {
    _z = z;
}

EntityOrientationEnum EntityStateDTO::orientation() const {
    return _orientation;
}

void EntityStateDTO::setOrientation( EntityOrientationEnum orientation ) {
    _orientation = orientation;
}

std::string EntityStateDTO::worldName() const {
    return _worldName;
}

void EntityStateDTO::setWorldName( const std::string& worldName ) {
    _worldName = worldName;
}

} // namespace Engine
