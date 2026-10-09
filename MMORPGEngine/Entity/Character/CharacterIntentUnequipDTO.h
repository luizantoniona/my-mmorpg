#ifndef CHARACTERINTENTUNEQUIPDTO_H
#define CHARACTERINTENTUNEQUIPDTO_H

#include <optional>

#include <json/json.h>

#include <MMORPGEngine/Entity/Character/EquipmentSlotEnum.h>

namespace Engine {

class CharacterIntentUnequipDTO {
public:
    CharacterIntentUnequipDTO();
    ~CharacterIntentUnequipDTO();

    static CharacterIntentUnequipDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    const std::optional<EquipmentSlotEnum>& slot() const;
    void setSlot( EquipmentSlotEnum slot );

private:
    std::optional<EquipmentSlotEnum> _slot;
};

} // namespace Engine

#endif // CHARACTERINTENTUNEQUIPDTO_H
