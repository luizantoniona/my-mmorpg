#include "CharacterModel.h"

namespace Engine {

CharacterModel::CharacterModel() :
    _idCharacter( 0 ),
    _idUser( 0 ),
    _name( "" ) {
}

int CharacterModel::idCharacter() const {
    return _idCharacter;
}

void CharacterModel::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

int CharacterModel::idUser() const {
    return _idUser;
}

void CharacterModel::setIdUser( int idUser ) {
    _idUser = idUser;
}

std::string CharacterModel::name() const {
    return _name;
}

void CharacterModel::setName( const std::string& name ) {
    _name = name;
}

} // namespace Engine
