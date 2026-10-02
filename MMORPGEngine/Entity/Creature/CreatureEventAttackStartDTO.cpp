#include "CreatureEventAttackStartDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

CreatureEventAttackStartDTO::CreatureEventAttackStartDTO() :
    _castSeconds( 0.0 ),
    _idCreature( 0 ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

CreatureEventAttackStartDTO::~CreatureEventAttackStartDTO() = default;

CreatureEventAttackStartDTO CreatureEventAttackStartDTO::fromJson( const Json::Value& json ) {
    CreatureEventAttackStartDTO dto;

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

    if ( json.isMember( "castSeconds" ) && json[ "castSeconds" ].isNumeric() ) {
        dto._castSeconds = json[ "castSeconds" ].asDouble();
    }

    return dto;
}

Json::Value CreatureEventAttackStartDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::CREATURE_EVENT_ATTACK_START );
    json[ "idCreature" ] = _idCreature;
    json[ "x" ] = _x;
    json[ "y" ] = _y;
    json[ "z" ] = _z;
    json[ "castSeconds" ] = _castSeconds;

    return json;
}

int CreatureEventAttackStartDTO::idCreature() const {
    return _idCreature;
}

void CreatureEventAttackStartDTO::setIdCreature( int idCreature ) {
    _idCreature = idCreature;
}

int CreatureEventAttackStartDTO::x() const {
    return _x;
}

void CreatureEventAttackStartDTO::setX( int x ) {
    _x = x;
}

int CreatureEventAttackStartDTO::y() const {
    return _y;
}

void CreatureEventAttackStartDTO::setY( int y ) {
    _y = y;
}

int CreatureEventAttackStartDTO::z() const {
    return _z;
}

void CreatureEventAttackStartDTO::setZ( int z ) {
    _z = z;
}

double CreatureEventAttackStartDTO::castSeconds() const {
    return _castSeconds;
}

void CreatureEventAttackStartDTO::setCastSeconds( double castSeconds ) {
    _castSeconds = castSeconds;
}

} // namespace Engine
