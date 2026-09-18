#include "CreatureModel.h"

namespace Engine {

CreatureModel::CreatureModel() :
    _idCreature( 0 ) {
}

int CreatureModel::idCreature() const {
    return _idCreature;
}

void CreatureModel::setIdCreature( int idCreature ) {
    _idCreature = idCreature;
}

} // namespace Engine
