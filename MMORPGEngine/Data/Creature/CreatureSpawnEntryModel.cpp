#include "CreatureSpawnEntryModel.h"

namespace Engine {

CreatureSpawnEntryModel::CreatureSpawnEntryModel() :
    _type( "" ),
    _quantity( 0 ) {
}

QString CreatureSpawnEntryModel::type() const {
    return _type;
}

void CreatureSpawnEntryModel::setType( const QString& type ) {
    _type = type;
}

uint32_t CreatureSpawnEntryModel::quantity() const {
    return _quantity;
}

void CreatureSpawnEntryModel::setQuantity( uint32_t quantity ) {
    _quantity = quantity;
}

} // namespace Engine
