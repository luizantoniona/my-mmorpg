#include "CreatureDTO.h"

#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

CreatureDTO::CreatureDTO() :
    _idCreature( 0 ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

CreatureDTO::~CreatureDTO() = default;

CreatureDTO CreatureDTO::fromModel( const CreatureModel* creature ) {
    CreatureDTO dto;

    const EntityPositionModel& position = creature->position();

    dto._idCreature = creature->idCreature();
    dto._x = position.x();
    dto._y = position.y();
    dto._z = position.z();

    return dto;
}

CreatureDTO CreatureDTO::fromJson( const Json::Value& json ) {
    CreatureDTO dto;

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

    return dto;
}

Json::Value CreatureDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::CREATURE );
    json[ "idCreature" ] = _idCreature;
    json[ "x" ] = _x;
    json[ "y" ] = _y;
    json[ "z" ] = _z;

    return json;
}

int CreatureDTO::idCreature() const {
    return _idCreature;
}

void CreatureDTO::setIdCreature( int idCreature ) {
    _idCreature = idCreature;
}

int CreatureDTO::x() const {
    return _x;
}

void CreatureDTO::setX( int x ) {
    _x = x;
}

int CreatureDTO::y() const {
    return _y;
}

void CreatureDTO::setY( int y ) {
    _y = y;
}

int CreatureDTO::z() const {
    return _z;
}

void CreatureDTO::setZ( int z ) {
    _z = z;
}

} // namespace Engine
