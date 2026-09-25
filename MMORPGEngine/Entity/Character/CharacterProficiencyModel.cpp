#include "CharacterProficiencyModel.h"

namespace Engine {

CharacterProficiencyModel::CharacterProficiencyModel() :
    _xp( 0.0 ),
    _idItemType( 0 ),
    _lvl( 0 ) {
}

uint32_t CharacterProficiencyModel::idItemType() const {
    return _idItemType;
}

void CharacterProficiencyModel::setIdItemType( uint32_t idItemType ) {
    _idItemType = idItemType;
}

double CharacterProficiencyModel::xp() const {
    return _xp;
}

void CharacterProficiencyModel::setXp( double xp ) {
    _xp = xp;
}

uint32_t CharacterProficiencyModel::lvl() const {
    return _lvl;
}

void CharacterProficiencyModel::setLvl( uint32_t lvl ) {
    _lvl = lvl;
}

} // namespace Engine
