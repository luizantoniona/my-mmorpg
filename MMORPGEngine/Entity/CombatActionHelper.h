#ifndef COMBATACTIONHELPER_H
#define COMBATACTIONHELPER_H

#include <optional>
#include <string>

#include <MMORPGEngine/Entity/CombatActionEnum.h>

namespace Engine {

class CombatActionHelper {
public:
    static std::string toString( CombatActionEnum action );
    static std::optional<CombatActionEnum> fromString( const std::string& value );
};

} // namespace Engine

#endif // COMBATACTIONHELPER_H
