#include "OwnCharacterVitalsDTO.h"

namespace Engine {

OwnCharacterVitalsDTO::OwnCharacterVitalsDTO() :
    _health( 0.0 ),
    _maxHealth( 0.0 ),
    _mana( 0.0 ),
    _maxMana( 0.0 ),
    _stamina( 0.0 ),
    _maxStamina( 0.0 ),
    _idCharacter( 0 ) {
}

OwnCharacterVitalsDTO::~OwnCharacterVitalsDTO() = default;

OwnCharacterVitalsDTO OwnCharacterVitalsDTO::fromJson( const Json::Value& json ) {
    OwnCharacterVitalsDTO dto;

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

Json::Value OwnCharacterVitalsDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = "own_character_vitals";
    json[ "idCharacter" ] = _idCharacter;
    json[ "health" ] = _health;
    json[ "maxHealth" ] = _maxHealth;
    json[ "mana" ] = _mana;
    json[ "maxMana" ] = _maxMana;
    json[ "stamina" ] = _stamina;
    json[ "maxStamina" ] = _maxStamina;

    return json;
}

int OwnCharacterVitalsDTO::idCharacter() const {
    return _idCharacter;
}

void OwnCharacterVitalsDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

double OwnCharacterVitalsDTO::health() const {
    return _health;
}

void OwnCharacterVitalsDTO::setHealth( double health ) {
    _health = health;
}

double OwnCharacterVitalsDTO::maxHealth() const {
    return _maxHealth;
}

void OwnCharacterVitalsDTO::setMaxHealth( double maxHealth ) {
    _maxHealth = maxHealth;
}

double OwnCharacterVitalsDTO::mana() const {
    return _mana;
}

void OwnCharacterVitalsDTO::setMana( double mana ) {
    _mana = mana;
}

double OwnCharacterVitalsDTO::maxMana() const {
    return _maxMana;
}

void OwnCharacterVitalsDTO::setMaxMana( double maxMana ) {
    _maxMana = maxMana;
}

double OwnCharacterVitalsDTO::stamina() const {
    return _stamina;
}

void OwnCharacterVitalsDTO::setStamina( double stamina ) {
    _stamina = stamina;
}

double OwnCharacterVitalsDTO::maxStamina() const {
    return _maxStamina;
}

void OwnCharacterVitalsDTO::setMaxStamina( double maxStamina ) {
    _maxStamina = maxStamina;
}

} // namespace Engine
