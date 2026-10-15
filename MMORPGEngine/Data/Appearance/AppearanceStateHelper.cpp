#include "AppearanceStateHelper.h"

namespace Engine {

std::string AppearanceStateHelper::toString( AppearanceStateEnum state ) {
    switch ( state ) {
    case AppearanceStateEnum::IDLE:
        return "idle";
    case AppearanceStateEnum::WALK:
        return "walk";
    case AppearanceStateEnum::ATTACK:
        return "attack";
    case AppearanceStateEnum::DODGE:
        return "dodge";
    case AppearanceStateEnum::BLOCK:
        return "block";
    case AppearanceStateEnum::DEAD:
        return "dead";
    case AppearanceStateEnum::UNKNOWN:
        return "";
    }

    return "";
}

AppearanceStateEnum AppearanceStateHelper::fromString( const std::string& value ) {
    if ( value == "idle" ) {
        return AppearanceStateEnum::IDLE;
    }
    if ( value == "walk" ) {
        return AppearanceStateEnum::WALK;
    }
    if ( value == "attack" ) {
        return AppearanceStateEnum::ATTACK;
    }
    if ( value == "dodge" ) {
        return AppearanceStateEnum::DODGE;
    }
    if ( value == "block" ) {
        return AppearanceStateEnum::BLOCK;
    }
    if ( value == "dead" ) {
        return AppearanceStateEnum::DEAD;
    }

    return AppearanceStateEnum::UNKNOWN;
}

} // namespace Engine
