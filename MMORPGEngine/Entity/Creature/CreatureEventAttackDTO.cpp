#include "CreatureEventAttackDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

CreatureEventAttackDTO::CreatureEventAttackDTO() :
    _idCreature( 0 ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

CreatureEventAttackDTO::~CreatureEventAttackDTO() = default;

CreatureEventAttackDTO CreatureEventAttackDTO::fromJson( const Json::Value& json ) {
    CreatureEventAttackDTO dto;

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

Json::Value CreatureEventAttackDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::CREATURE_EVENT_ATTACK );
    json[ "idCreature" ] = _idCreature;
    json[ "x" ] = _x;
    json[ "y" ] = _y;
    json[ "z" ] = _z;

    return json;
}

int CreatureEventAttackDTO::idCreature() const {
    return _idCreature;
}

void CreatureEventAttackDTO::setIdCreature( int idCreature ) {
    _idCreature = idCreature;
}

int CreatureEventAttackDTO::x() const {
    return _x;
}

void CreatureEventAttackDTO::setX( int x ) {
    _x = x;
}

int CreatureEventAttackDTO::y() const {
    return _y;
}

void CreatureEventAttackDTO::setY( int y ) {
    _y = y;
}

int CreatureEventAttackDTO::z() const {
    return _z;
}

void CreatureEventAttackDTO::setZ( int z ) {
    _z = z;
}

} // namespace Engine
