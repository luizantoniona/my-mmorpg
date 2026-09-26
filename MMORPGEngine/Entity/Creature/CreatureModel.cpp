#include "CreatureModel.h"

namespace Engine {

CreatureModel::CreatureModel() :
    _idCreature( 0 ),
    _type( 0 ) {
}

int CreatureModel::idCreature() const {
    return _idCreature;
}

void CreatureModel::setIdCreature( int idCreature ) {
    _idCreature = idCreature;
}

uint32_t CreatureModel::type() const {
    return _type;
}

void CreatureModel::setType( uint32_t type ) {
    _type = type;
}

} // namespace Engine
