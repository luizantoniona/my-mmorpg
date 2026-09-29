#include "CreatureDTO.h"

#include <MMORPGEngine/Entity/EntityMovementModel.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/Entity/EntityVitalsModel.h>
#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

CreatureDTO::CreatureDTO() :
    _health( 0.0 ),
    _maxHealth( 0.0 ),
    _movementCooldownSeconds( 0.0 ),
    _idCreature( 0 ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

CreatureDTO::~CreatureDTO() = default;

CreatureDTO CreatureDTO::fromModel( const CreatureModel* creature ) {
    CreatureDTO dto;

    const EntityPositionModel& position = creature->position();
    const EntityVitalsModel& vitals = creature->vitals();

    dto._idCreature = creature->idCreature();
    dto._x = position.x();
    dto._y = position.y();
    dto._z = position.z();
    dto._health = vitals.health();
    dto._maxHealth = vitals.maxHealth();
    dto._movementCooldownSeconds = creature->movement().cooldownSeconds();

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

    if ( json.isMember( "health" ) && json[ "health" ].isNumeric() ) {
        dto._health = json[ "health" ].asDouble();
    }

    if ( json.isMember( "maxHealth" ) && json[ "maxHealth" ].isNumeric() ) {
        dto._maxHealth = json[ "maxHealth" ].asDouble();
    }

    if ( json.isMember( "movementCooldownSeconds" ) && json[ "movementCooldownSeconds" ].isNumeric() ) {
        dto._movementCooldownSeconds = json[ "movementCooldownSeconds" ].asDouble();
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
    json[ "health" ] = _health;
    json[ "maxHealth" ] = _maxHealth;
    json[ "movementCooldownSeconds" ] = _movementCooldownSeconds;

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

double CreatureDTO::health() const {
    return _health;
}

void CreatureDTO::setHealth( double health ) {
    _health = health;
}

double CreatureDTO::maxHealth() const {
    return _maxHealth;
}

void CreatureDTO::setMaxHealth( double maxHealth ) {
    _maxHealth = maxHealth;
}

double CreatureDTO::movementCooldownSeconds() const {
    return _movementCooldownSeconds;
}

void CreatureDTO::setMovementCooldownSeconds( double movementCooldownSeconds ) {
    _movementCooldownSeconds = movementCooldownSeconds;
}

} // namespace Engine
