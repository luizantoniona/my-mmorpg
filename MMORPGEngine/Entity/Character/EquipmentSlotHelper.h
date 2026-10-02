#ifndef EQUIPMENTSLOTHELPER_H
#define EQUIPMENTSLOTHELPER_H

#include <optional>
#include <string>

#include <MMORPGEngine/Entity/Character/EquipmentSlotEnum.h>

namespace Engine {

class EquipmentSlotHelper {
public:
    static std::string toString( EquipmentSlotEnum slot );
    static std::optional<EquipmentSlotEnum> fromString( const std::string& value );
};

} // namespace Engine

#endif // EQUIPMENTSLOTHELPER_H
