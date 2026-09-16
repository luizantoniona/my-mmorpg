#include "EntityOrientationModel.h"

namespace Engine {

EntityOrientationModel::EntityOrientationModel() :
    _direction( EntityOrientationEnum::SOUTH ) {
}

EntityOrientationModel::~EntityOrientationModel() = default;

EntityOrientationEnum EntityOrientationModel::fromMovement( int dx, int dy ) {
    if ( dx == 0 && dy < 0 ) {
        return EntityOrientationEnum::NORTH;
    }
    if ( dx > 0 && dy < 0 ) {
        return EntityOrientationEnum::NORTHEAST;
    }
    if ( dx > 0 && dy == 0 ) {
        return EntityOrientationEnum::EAST;
    }
    if ( dx > 0 && dy > 0 ) {
        return EntityOrientationEnum::SOUTHEAST;
    }
    if ( dx == 0 && dy > 0 ) {
        return EntityOrientationEnum::SOUTH;
    }
    if ( dx < 0 && dy > 0 ) {
        return EntityOrientationEnum::SOUTHWEST;
    }
    if ( dx < 0 && dy == 0 ) {
        return EntityOrientationEnum::WEST;
    }
    if ( dx < 0 && dy < 0 ) {
        return EntityOrientationEnum::NORTHWEST;
    }

    return EntityOrientationEnum::SOUTH;
}

std::string EntityOrientationModel::toString( EntityOrientationEnum orientation ) {
    switch ( orientation ) {
    case EntityOrientationEnum::NORTH:
        return "north";
    case EntityOrientationEnum::NORTHEAST:
        return "northeast";
    case EntityOrientationEnum::EAST:
        return "east";
    case EntityOrientationEnum::SOUTHEAST:
        return "southeast";
    case EntityOrientationEnum::SOUTH:
        return "south";
    case EntityOrientationEnum::SOUTHWEST:
        return "southwest";
    case EntityOrientationEnum::WEST:
        return "west";
    case EntityOrientationEnum::NORTHWEST:
        return "northwest";
    }

    return "south";
}

EntityOrientationEnum EntityOrientationModel::fromString( const std::string& value ) {
    if ( value == "north" ) {
        return EntityOrientationEnum::NORTH;
    }
    if ( value == "northeast" ) {
        return EntityOrientationEnum::NORTHEAST;
    }
    if ( value == "east" ) {
        return EntityOrientationEnum::EAST;
    }
    if ( value == "southeast" ) {
        return EntityOrientationEnum::SOUTHEAST;
    }
    if ( value == "southwest" ) {
        return EntityOrientationEnum::SOUTHWEST;
    }
    if ( value == "west" ) {
        return EntityOrientationEnum::WEST;
    }
    if ( value == "northwest" ) {
        return EntityOrientationEnum::NORTHWEST;
    }

    return EntityOrientationEnum::SOUTH;
}

EntityOrientationEnum EntityOrientationModel::direction() const {
    return _direction;
}

void EntityOrientationModel::setDirection( EntityOrientationEnum direction ) {
    _direction = direction;
}

} // namespace Engine
