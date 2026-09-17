#include "CharacterDTO.h"

#include <MMORPGEngine/Network/WebSocket/NetworkMessageTypeHelper.h>

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
    _z( 0 ),
    _orientation( EntityOrientationEnum::SOUTH ) {
}

CharacterDTO::~CharacterDTO() = default;

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

    if ( json.isMember( "orientation" ) && json[ "orientation" ].isInt() ) {
        dto._orientation = static_cast<EntityOrientationEnum>( json[ "orientation" ].asInt() );
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

    json[ "type" ] = NetworkMessageTypeHelper::toString( NetworkMessageType::CHARACTER );
    json[ "idCharacter" ] = _idCharacter;
    json[ "x" ] = _x;
    json[ "y" ] = _y;
    json[ "z" ] = _z;
    json[ "orientation" ] = static_cast<int>( _orientation );
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

EntityOrientationEnum CharacterDTO::orientation() const {
    return _orientation;
}

void CharacterDTO::setOrientation( EntityOrientationEnum orientation ) {
    _orientation = orientation;
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
