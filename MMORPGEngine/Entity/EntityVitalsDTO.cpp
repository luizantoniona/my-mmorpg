#include "EntityVitalsDTO.h"

namespace Engine {

EntityVitalsDTO::EntityVitalsDTO() :
    _idCharacter( 0 ),
    _health( 0.0 ),
    _maxHealth( 0.0 ),
    _mana( 0.0 ),
    _maxMana( 0.0 ),
    _stamina( 0.0 ),
    _maxStamina( 0.0 ) {
}

EntityVitalsDTO::~EntityVitalsDTO() = default;

EntityVitalsDTO EntityVitalsDTO::fromJson( const Json::Value& json ) {
    EntityVitalsDTO dto;

    if ( json.isMember( "idCharacter" ) && json[ "idCharacter" ].isInt() ) {
        dto._idCharacter = json[ "idCharacter" ].asInt();
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

Json::Value EntityVitalsDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = "vitals";
    json[ "idCharacter" ] = _idCharacter;
    json[ "health" ] = _health;
    json[ "maxHealth" ] = _maxHealth;
    json[ "mana" ] = _mana;
    json[ "maxMana" ] = _maxMana;
    json[ "stamina" ] = _stamina;
    json[ "maxStamina" ] = _maxStamina;

    return json;
}

int EntityVitalsDTO::idCharacter() const {
    return _idCharacter;
}

void EntityVitalsDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

double EntityVitalsDTO::health() const {
    return _health;
}

void EntityVitalsDTO::setHealth( double health ) {
    _health = health;
}

double EntityVitalsDTO::maxHealth() const {
    return _maxHealth;
}

void EntityVitalsDTO::setMaxHealth( double maxHealth ) {
    _maxHealth = maxHealth;
}

double EntityVitalsDTO::mana() const {
    return _mana;
}

void EntityVitalsDTO::setMana( double mana ) {
    _mana = mana;
}

double EntityVitalsDTO::maxMana() const {
    return _maxMana;
}

void EntityVitalsDTO::setMaxMana( double maxMana ) {
    _maxMana = maxMana;
}

double EntityVitalsDTO::stamina() const {
    return _stamina;
}

void EntityVitalsDTO::setStamina( double stamina ) {
    _stamina = stamina;
}

double EntityVitalsDTO::maxStamina() const {
    return _maxStamina;
}

void EntityVitalsDTO::setMaxStamina( double maxStamina ) {
    _maxStamina = maxStamina;
}

} // namespace Engine
