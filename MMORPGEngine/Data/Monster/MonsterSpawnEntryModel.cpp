#include "MonsterSpawnEntryModel.h"

namespace Engine {

MonsterSpawnEntryModel::MonsterSpawnEntryModel() :
    _type( 0 ),
    _quantity( 0 ) {
}

uint32_t MonsterSpawnEntryModel::type() const {
    return _type;
}

void MonsterSpawnEntryModel::setType( uint32_t type ) {
    _type = type;
}

uint32_t MonsterSpawnEntryModel::quantity() const {
    return _quantity;
}

void MonsterSpawnEntryModel::setQuantity( uint32_t quantity ) {
    _quantity = quantity;
}

} // namespace Engine
