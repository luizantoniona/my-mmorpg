#include "CharacterModel.h"

namespace Engine {

CharacterModel::CharacterModel() :
    _idCharacter( 0 ),
    _idAccount( 0 ),
    _name( "" ) {
}

int CharacterModel::idCharacter() const {
    return _idCharacter;
}

void CharacterModel::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

int CharacterModel::idAccount() const {
    return _idAccount;
}

void CharacterModel::setIdAccount( int idAccount ) {
    _idAccount = idAccount;
}

std::string CharacterModel::name() const {
    return _name;
}

void CharacterModel::setName( const std::string& name ) {
    _name = name;
}

} // namespace Engine
