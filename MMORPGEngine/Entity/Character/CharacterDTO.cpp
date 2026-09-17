#include "CharacterDTO.h"

#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/Entity/EntityVitalsModel.h>
#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

CharacterDTO::CharacterDTO() :
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

CharacterDTO::~CharacterDTO() = default;

CharacterDTO CharacterDTO::fromModel( const CharacterModel* character ) {
    CharacterDTO dto;

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

CharacterDTO CharacterDTO::fromJson( const Json::Value& json ) {
    CharacterDTO dto;

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

Json::Value CharacterDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::CHARACTER );
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

int CharacterDTO::idCharacter() const {
    return _idCharacter;
}

void CharacterDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

int CharacterDTO::x() const {
    return _x;
}

void CharacterDTO::setX( int x ) {
    _x = x;
}

int CharacterDTO::y() const {
    return _y;
}

void CharacterDTO::setY( int y ) {
    _y = y;
}

int CharacterDTO::z() const {
    return _z;
}

void CharacterDTO::setZ( int z ) {
    _z = z;
}

double CharacterDTO::health() const {
    return _health;
}

void CharacterDTO::setHealth( double health ) {
    _health = health;
}

double CharacterDTO::maxHealth() const {
    return _maxHealth;
}

void CharacterDTO::setMaxHealth( double maxHealth ) {
    _maxHealth = maxHealth;
}

double CharacterDTO::mana() const {
    return _mana;
}

void CharacterDTO::setMana( double mana ) {
    _mana = mana;
}

double CharacterDTO::maxMana() const {
    return _maxMana;
}

void CharacterDTO::setMaxMana( double maxMana ) {
    _maxMana = maxMana;
}

double CharacterDTO::stamina() const {
    return _stamina;
}

void CharacterDTO::setStamina( double stamina ) {
    _stamina = stamina;
}

double CharacterDTO::maxStamina() const {
    return _maxStamina;
}

void CharacterDTO::setMaxStamina( double maxStamina ) {
    _maxStamina = maxStamina;
}

} // namespace Engine
