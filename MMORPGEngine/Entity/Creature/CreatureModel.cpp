#include "CreatureModel.h"

namespace Engine {

CreatureModel::CreatureModel() :
    _type( "" ),
    _idCreature( 0 ) {
}

int CreatureModel::idCreature() const {
    return _idCreature;
}

void CreatureModel::setIdCreature( int idCreature ) {
    _idCreature = idCreature;
}

QString CreatureModel::type() const {
    return _type;
}

void CreatureModel::setType( const QString& type ) {
    _type = type;
}

} // namespace Engine
