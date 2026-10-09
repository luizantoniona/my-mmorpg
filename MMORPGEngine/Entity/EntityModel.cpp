#include "EntityModel.h"

namespace Engine {

EntityModel::EntityModel() :
    _position(),
    _size(),
    _vitals(),
    _movement(),
    _combat() {
}

EntityModel::~EntityModel() = default;

EntityPositionModel& EntityModel::position() {
    return _position;
}

const EntityPositionModel& EntityModel::position() const {
    return _position;
}

EntitySizeModel& EntityModel::size() {
    return _size;
}

const EntitySizeModel& EntityModel::size() const {
    return _size;
}

EntityVitalsModel& EntityModel::vitals() {
    return _vitals;
}

const EntityVitalsModel& EntityModel::vitals() const {
    return _vitals;
}

EntityMovementModel& EntityModel::movement() {
    return _movement;
}

const EntityMovementModel& EntityModel::movement() const {
    return _movement;
}

EntityCombatModel& EntityModel::combat() {
    return _combat;
}

const EntityCombatModel& EntityModel::combat() const {
    return _combat;
}

bool EntityModel::occupies( int x, int y, int z ) const {
    return overlaps( x, y, z, 1, 1 );
}

bool EntityModel::overlaps( int x, int y, int z, int width, int height ) const {
    if ( _position.z() != z ) {
        return false;
    }

    const bool overlapsX = x < _position.x() + _size.width() && _position.x() < x + width;
    const bool overlapsY = y < _position.y() + _size.height() && _position.y() < y + height;

    return overlapsX && overlapsY;
}

} // namespace Engine
