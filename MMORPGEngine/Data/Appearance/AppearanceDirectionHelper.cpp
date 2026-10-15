#include "AppearanceDirectionHelper.h"

namespace Engine {

std::string AppearanceDirectionHelper::toString( AppearanceDirectionEnum direction ) {
    switch ( direction ) {
    case AppearanceDirectionEnum::NORTH:
        return "north";
    case AppearanceDirectionEnum::SOUTH:
        return "south";
    case AppearanceDirectionEnum::EAST:
        return "east";
    case AppearanceDirectionEnum::WEST:
        return "west";
    case AppearanceDirectionEnum::UNKNOWN:
        return "";
    }

    return "";
}

AppearanceDirectionEnum AppearanceDirectionHelper::fromString( const std::string& value ) {
    if ( value == "north" ) {
        return AppearanceDirectionEnum::NORTH;
    }
    if ( value == "south" ) {
        return AppearanceDirectionEnum::SOUTH;
    }
    if ( value == "east" ) {
        return AppearanceDirectionEnum::EAST;
    }
    if ( value == "west" ) {
        return AppearanceDirectionEnum::WEST;
    }

    return AppearanceDirectionEnum::UNKNOWN;
}

AppearanceDirectionEnum AppearanceDirectionHelper::fromDelta( int dx, int dy, AppearanceDirectionEnum fallback ) {
    if ( dx > 0 ) {
        return AppearanceDirectionEnum::EAST;
    }
    if ( dx < 0 ) {
        return AppearanceDirectionEnum::WEST;
    }
    if ( dy > 0 ) {
        return AppearanceDirectionEnum::SOUTH;
    }
    if ( dy < 0 ) {
        return AppearanceDirectionEnum::NORTH;
    }

    return fallback;
}

} // namespace Engine
