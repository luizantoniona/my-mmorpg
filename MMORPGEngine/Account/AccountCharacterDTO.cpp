#include "AccountCharacterDTO.h"

namespace Engine {

AccountCharacterDTO::AccountCharacterDTO() :
    _idCharacter( 0 ),
    _name( "" ) {
}

AccountCharacterDTO::AccountCharacterDTO( const AccountCharacterModel& character ) :
    _idCharacter( character.idCharacter() ),
    _name( character.name() ) {
}

AccountCharacterDTO::~AccountCharacterDTO() = default;

AccountCharacterDTO AccountCharacterDTO::fromJson( const Json::Value& json ) {
    AccountCharacterDTO dto;

    if ( json.isMember( "idCharacter" ) && json[ "idCharacter" ].isInt() ) {
        dto._idCharacter = json[ "idCharacter" ].asInt();
    }

    if ( json.isMember( "name" ) && json[ "name" ].isString() ) {
        dto._name = json[ "name" ].asString();
    }

    return dto;
}

Json::Value AccountCharacterDTO::toJson() const {
    Json::Value json;

    json[ "idCharacter" ] = _idCharacter;
    json[ "name" ] = _name;

    return json;
}

int AccountCharacterDTO::idCharacter() const {
    return _idCharacter;
}

void AccountCharacterDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

std::string AccountCharacterDTO::name() const {
    return _name;
}

void AccountCharacterDTO::setName( const std::string& name ) {
    _name = name;
}

} // namespace Engine
