#include "CharacterInventoryModel.h"

namespace Engine {

CharacterInventoryModel::CharacterInventoryModel() :
    _position( 0 ),
    _idItem( 0 ),
    _amount( 0 ) {
}

uint32_t CharacterInventoryModel::position() const {
    return _position;
}

void CharacterInventoryModel::setPosition( uint32_t position ) {
    _position = position;
}

uint32_t CharacterInventoryModel::idItem() const {
    return _idItem;
}

void CharacterInventoryModel::setIdItem( uint32_t idItem ) {
    _idItem = idItem;
}

uint32_t CharacterInventoryModel::amount() const {
    return _amount;
}

void CharacterInventoryModel::setAmount( uint32_t amount ) {
    _amount = amount;
}

} // namespace Engine
