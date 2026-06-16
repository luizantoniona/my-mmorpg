#include "CharacterDTO.h"

namespace Engine {

CharacterDTO::CharacterDTO() :
    _idCharacter( 0 ),
    _idAccount( 0 ),
    _name( "" ) {}

CharacterDTO::CharacterDTO( const CharacterModel& character ) :
    _idCharacter( character.idCharacter() ),
    _idAccount( character.idAccount() ),
    _name( character.name() ) {}

Json::Value CharacterDTO::toJson() const {
    Json::Value json;
    json[ "idCharacter" ] = _idCharacter;
    json[ "idAccount" ] = _idAccount;
    json[ "name" ] = _name;
    return json;
}

int CharacterDTO::idCharacter() const {
    return _idCharacter;
}

void CharacterDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

int CharacterDTO::idAccount() const {
    return _idAccount;
}

void CharacterDTO::setIdAccount( int idAccount ) {
    _idAccount = idAccount;
}

std::string CharacterDTO::name() const {
    return _name;
}

void CharacterDTO::setName( const std::string& name ) {
    _name = name;
}

} // namespace Engine
