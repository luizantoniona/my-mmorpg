#include "AttackHitModeHelper.h"

namespace Engine {

std::string AttackHitModeHelper::toString( AttackHitModeEnum hitMode ) {
    switch ( hitMode ) {
    case AttackHitModeEnum::ALL:
        return "ALL";
    case AttackHitModeEnum::FIRST:
        return "FIRST";
    case AttackHitModeEnum::UNKNOWN:
        break;
    }

    return "";
}

AttackHitModeEnum AttackHitModeHelper::fromString( const std::string& value ) {
    if ( value == "ALL" ) {
        return AttackHitModeEnum::ALL;
    }
    if ( value == "FIRST" ) {
        return AttackHitModeEnum::FIRST;
    }

    return AttackHitModeEnum::UNKNOWN;
}

} // namespace Engine
