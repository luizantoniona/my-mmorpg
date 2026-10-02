#ifndef CHARACTEREQUIPMENTMODEL_H
#define CHARACTEREQUIPMENTMODEL_H

#include <cstdint>

#include <MMORPGEngine/Entity/Character/EquipmentSlotEnum.h>

namespace Engine {

class CharacterEquipmentModel {
public:
    CharacterEquipmentModel();

    EquipmentSlotEnum slot() const;
    void setSlot( EquipmentSlotEnum slot );

    uint32_t idItem() const;
    void setIdItem( uint32_t idItem );

private:
    EquipmentSlotEnum _slot;
    uint32_t _idItem;
};

} // namespace Engine

#endif // CHARACTEREQUIPMENTMODEL_H
