#ifndef CHARACTEREQUIPMENTDTO_H
#define CHARACTEREQUIPMENTDTO_H

#include <cstdint>

#include <json/json.h>

#include <MMORPGEngine/Entity/Character/CharacterEquipmentModel.h>

namespace Engine {

class CharacterEquipmentDTO {
public:
    CharacterEquipmentDTO();

    static CharacterEquipmentDTO fromModel( const CharacterEquipmentModel& equipment );
    static CharacterEquipmentDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    EquipmentSlotEnum slot() const;
    void setSlot( EquipmentSlotEnum slot );

    uint32_t idItem() const;
    void setIdItem( uint32_t idItem );

private:
    EquipmentSlotEnum _slot;
    uint32_t _idItem;
};

} // namespace Engine

#endif // CHARACTEREQUIPMENTDTO_H
