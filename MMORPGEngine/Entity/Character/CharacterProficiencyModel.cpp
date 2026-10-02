#include "CharacterProficiencyModel.h"

namespace Engine {

CharacterProficiencyModel::CharacterProficiencyModel() :
    _xp( 0.0 ),
    _idItemType( "" ),
    _lvl( 0 ) {
}

QString CharacterProficiencyModel::idItemType() const {
    return _idItemType;
}

void CharacterProficiencyModel::setIdItemType( const QString& idItemType ) {
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
