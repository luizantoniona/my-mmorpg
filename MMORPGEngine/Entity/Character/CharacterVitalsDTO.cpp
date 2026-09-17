#include "CharacterVitalsDTO.h"

namespace Engine {

CharacterVitalsDTO::CharacterVitalsDTO() :
    _health( 0.0 ),
    _maxHealth( 0.0 ),
    _mana( 0.0 ),
    _maxMana( 0.0 ),
    _stamina( 0.0 ),
    _maxStamina( 0.0 ),
    _idCharacter( 0 ) {
}

CharacterVitalsDTO::~CharacterVitalsDTO() = default;

CharacterVitalsDTO CharacterVitalsDTO::fromJson( const Json::Value& json ) {
    CharacterVitalsDTO dto;

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

Json::Value CharacterVitalsDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = "character_vitals";
    json[ "idCharacter" ] = _idCharacter;
    json[ "health" ] = _health;
    json[ "maxHealth" ] = _maxHealth;
    json[ "mana" ] = _mana;
    json[ "maxMana" ] = _maxMana;
    json[ "stamina" ] = _stamina;
    json[ "maxStamina" ] = _maxStamina;

    return json;
}

int CharacterVitalsDTO::idCharacter() const {
    return _idCharacter;
}

void CharacterVitalsDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

double CharacterVitalsDTO::health() const {
    return _health;
}

void CharacterVitalsDTO::setHealth( double health ) {
    _health = health;
}

double CharacterVitalsDTO::maxHealth() const {
    return _maxHealth;
}

void CharacterVitalsDTO::setMaxHealth( double maxHealth ) {
    _maxHealth = maxHealth;
}

double CharacterVitalsDTO::mana() const {
    return _mana;
}

void CharacterVitalsDTO::setMana( double mana ) {
    _mana = mana;
}

double CharacterVitalsDTO::maxMana() const {
    return _maxMana;
}

void CharacterVitalsDTO::setMaxMana( double maxMana ) {
    _maxMana = maxMana;
}

double CharacterVitalsDTO::stamina() const {
    return _stamina;
}

void CharacterVitalsDTO::setStamina( double stamina ) {
    _stamina = stamina;
}

double CharacterVitalsDTO::maxStamina() const {
    return _maxStamina;
}

void CharacterVitalsDTO::setMaxStamina( double maxStamina ) {
    _maxStamina = maxStamina;
}

} // namespace Engine
