#include "EntityAttackedDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

EntityAttackedDTO::EntityAttackedDTO() :
    _idCharacter( 0 ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

EntityAttackedDTO::~EntityAttackedDTO() = default;

EntityAttackedDTO EntityAttackedDTO::fromJson( const Json::Value& json ) {
    EntityAttackedDTO dto;

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

Json::Value EntityAttackedDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::ENTITY_ATTACKED );
    json[ "idCharacter" ] = _idCharacter;
    json[ "x" ] = _x;
    json[ "y" ] = _y;
    json[ "z" ] = _z;

    return json;
}

int EntityAttackedDTO::idCharacter() const {
    return _idCharacter;
}

void EntityAttackedDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

int EntityAttackedDTO::x() const {
    return _x;
}

void EntityAttackedDTO::setX( int x ) {
    _x = x;
}

int EntityAttackedDTO::y() const {
    return _y;
}

void EntityAttackedDTO::setY( int y ) {
    _y = y;
}

int EntityAttackedDTO::z() const {
    return _z;
}

void EntityAttackedDTO::setZ( int z ) {
    _z = z;
}

} // namespace Engine
