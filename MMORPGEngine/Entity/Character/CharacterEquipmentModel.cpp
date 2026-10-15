#include "CharacterEquipmentModel.h"

namespace Engine {

CharacterEquipmentModel::CharacterEquipmentModel() :
    _idItem( "" ),
    _slot( EquipmentSlotEnum::NONE ) {
}

EquipmentSlotEnum CharacterEquipmentModel::slot() const {
    return _slot;
}

void CharacterEquipmentModel::setSlot( EquipmentSlotEnum slot ) {
    _slot = slot;
}

QString CharacterEquipmentModel::idItem() const {
    return _idItem;
}

void CharacterEquipmentModel::setIdItem( const QString& idItem ) {
    _idItem = idItem;
}

} // namespace Engine
