#include "CombatActionHelper.h"

namespace Engine {

std::string CombatActionHelper::toString( CombatActionEnum action ) {
    switch ( action ) {
    case CombatActionEnum::ATTACK:
        return "ATTACK";
    case CombatActionEnum::DODGE:
        return "DODGE";
    case CombatActionEnum::BLOCK:
        return "BLOCK";
    }

    return "";
}

std::optional<CombatActionEnum> CombatActionHelper::fromString( const std::string& value ) {
    if ( value == "ATTACK" ) {
        return CombatActionEnum::ATTACK;
    }
    if ( value == "DODGE" ) {
        return CombatActionEnum::DODGE;
    }
    if ( value == "BLOCK" ) {
        return CombatActionEnum::BLOCK;
    }

    return std::nullopt;
}

} // namespace Engine
