#include "CreatureSpawnEntryModel.h"

namespace Engine {

CreatureSpawnEntryModel::CreatureSpawnEntryModel() :
    _type( 0 ),
    _quantity( 0 ) {
}

uint32_t CreatureSpawnEntryModel::type() const {
    return _type;
}

void CreatureSpawnEntryModel::setType( uint32_t type ) {
    _type = type;
}

uint32_t CreatureSpawnEntryModel::quantity() const {
    return _quantity;
}

void CreatureSpawnEntryModel::setQuantity( uint32_t quantity ) {
    _quantity = quantity;
}

} // namespace Engine
