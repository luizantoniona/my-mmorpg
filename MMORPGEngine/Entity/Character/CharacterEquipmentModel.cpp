#include "CharacterEquipmentModel.h"

namespace Engine {

CharacterEquipmentModel::CharacterEquipmentModel() :
    _slot( EquipmentSlotEnum::NONE ),
    _idItem( 0 ) {
}

EquipmentSlotEnum CharacterEquipmentModel::slot() const {
    return _slot;
}

void CharacterEquipmentModel::setSlot( EquipmentSlotEnum slot ) {
    _slot = slot;
}

uint32_t CharacterEquipmentModel::idItem() const {
    return _idItem;
}

void CharacterEquipmentModel::setIdItem( uint32_t idItem ) {
    _idItem = idItem;
}

} // namespace Engine
