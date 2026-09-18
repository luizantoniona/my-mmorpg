#include "OwnCharacterDTO.h"

#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/Entity/EntityVitalsModel.h>
#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

OwnCharacterDTO::OwnCharacterDTO() :
    _health( 0.0 ),
    _maxHealth( 0.0 ),
    _mana( 0.0 ),
    _maxMana( 0.0 ),
    _stamina( 0.0 ),
    _maxStamina( 0.0 ),
    _idCharacter( 0 ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

OwnCharacterDTO::~OwnCharacterDTO() = default;

OwnCharacterDTO OwnCharacterDTO::fromModel( const CharacterModel* character ) {
    OwnCharacterDTO dto;

    const EntityPositionModel& position = character->position();
    const EntityVitalsModel& vitals = character->vitals();

    dto._idCharacter = character->idCharacter();
    dto._x = position.x();
    dto._y = position.y();
    dto._z = position.z();
    dto._health = vitals.health();
    dto._maxHealth = vitals.maxHealth();
    dto._mana = vitals.mana();
    dto._maxMana = vitals.maxMana();
    dto._stamina = vitals.stamina();
    dto._maxStamina = vitals.maxStamina();

    return dto;
}

OwnCharacterDTO OwnCharacterDTO::fromJson( const Json::Value& json ) {
    OwnCharacterDTO dto;

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

    if ( json.isMember( "health" ) && json[ "health" ].isNumeric() ) {
        dto._health = json[ "health" ].asDouble();
    }

    if ( json.isMember( "maxHealth" ) && json[ "maxHealth" ].isNumeric() ) {
        dto._maxHealth = json[ "maxHealth" ].asDouble();
    }

    if ( json.isMember( "mana" ) && json[ "mana" ].isNumeric() ) {
        dto._mana = json[ "mana" ].asDouble();
    }

    if ( json.isMember( "maxMana" ) && json[ "maxMana" ].isNumeric() ) {
        dto._maxMana = json[ "maxMana" ].asDouble();
    }

    if ( json.isMember( "stamina" ) && json[ "stamina" ].isNumeric() ) {
        dto._stamina = json[ "stamina" ].asDouble();
    }

    if ( json.isMember( "maxStamina" ) && json[ "maxStamina" ].isNumeric() ) {
        dto._maxStamina = json[ "maxStamina" ].asDouble();
    }

    return dto;
}

Json::Value OwnCharacterDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::OWN_CHARACTER );
    json[ "idCharacter" ] = _idCharacter;
    json[ "x" ] = _x;
    json[ "y" ] = _y;
    json[ "z" ] = _z;
    json[ "health" ] = _health;
    json[ "maxHealth" ] = _maxHealth;
    json[ "mana" ] = _mana;
    json[ "maxMana" ] = _maxMana;
    json[ "stamina" ] = _stamina;
    json[ "maxStamina" ] = _maxStamina;

    return json;
}

int OwnCharacterDTO::idCharacter() const {
    return _idCharacter;
}

void OwnCharacterDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

int OwnCharacterDTO::x() const {
    return _x;
}

void OwnCharacterDTO::setX( int x ) {
    _x = x;
}

int OwnCharacterDTO::y() const {
    return _y;
}

void OwnCharacterDTO::setY( int y ) {
    _y = y;
}

int OwnCharacterDTO::z() const {
    return _z;
}

void OwnCharacterDTO::setZ( int z ) {
    _z = z;
}

double OwnCharacterDTO::health() const {
    return _health;
}

void OwnCharacterDTO::setHealth( double health ) {
    _health = health;
}

double OwnCharacterDTO::maxHealth() const {
    return _maxHealth;
}

void OwnCharacterDTO::setMaxHealth( double maxHealth ) {
    _maxHealth = maxHealth;
}

double OwnCharacterDTO::mana() const {
    return _mana;
}

void OwnCharacterDTO::setMana( double mana ) {
    _mana = mana;
}

double OwnCharacterDTO::maxMana() const {
    return _maxMana;
}

void OwnCharacterDTO::setMaxMana( double maxMana ) {
    _maxMana = maxMana;
}

double OwnCharacterDTO::stamina() const {
    return _stamina;
}

void OwnCharacterDTO::setStamina( double stamina ) {
    _stamina = stamina;
}

double OwnCharacterDTO::maxStamina() const {
    return _maxStamina;
}

void OwnCharacterDTO::setMaxStamina( double maxStamina ) {
    _maxStamina = maxStamina;
}

} // namespace Engine
