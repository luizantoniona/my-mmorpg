#include "CharacterInventoryModel.h"

namespace Engine {

CharacterInventoryModel::CharacterInventoryModel() :
    _idItem( "" ),
    _position( 0 ),
    _amount( 0 ) {
}

uint32_t CharacterInventoryModel::position() const {
    return _position;
}

void CharacterInventoryModel::setPosition( uint32_t position ) {
    _position = position;
}

QString CharacterInventoryModel::idItem() const {
    return _idItem;
}

void CharacterInventoryModel::setIdItem( const QString& idItem ) {
    _idItem = idItem;
}

uint32_t CharacterInventoryModel::amount() const {
    return _amount;
}

void CharacterInventoryModel::setAmount( uint32_t amount ) {
    _amount = amount;
}

} // namespace Engine
