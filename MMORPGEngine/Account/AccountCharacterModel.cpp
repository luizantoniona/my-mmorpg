#include "AccountCharacterModel.h"

namespace Engine {

AccountCharacterModel::AccountCharacterModel() :
    _idCharacter( 0 ),
    _idAccount( 0 ),
    _name( "" ) {
}

AccountCharacterModel::~AccountCharacterModel() = default;

int AccountCharacterModel::idCharacter() const {
    return _idCharacter;
}

void AccountCharacterModel::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

int AccountCharacterModel::idAccount() const {
    return _idAccount;
}

void AccountCharacterModel::setIdAccount( int idAccount ) {
    _idAccount = idAccount;
}

std::string AccountCharacterModel::name() const {
    return _name;
}

void AccountCharacterModel::setName( const std::string& name ) {
    _name = name;
}

} // namespace Engine
