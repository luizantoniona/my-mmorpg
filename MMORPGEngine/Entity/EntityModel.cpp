#include "EntityModel.h"

namespace Engine {

EntityModel::EntityModel() :
    _position(),
    _size(),
    _vitals(),
    _movement() {
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

} // namespace Engine
