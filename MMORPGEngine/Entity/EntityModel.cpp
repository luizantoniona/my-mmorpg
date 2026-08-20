#include "EntityModel.h"

namespace Engine {

EntityModel::EntityModel() :
    _orientation(),
    _position(),
    _size() {
}

EntityModel::~EntityModel() = default;

EntityOrientationModel EntityModel::orientation() const {
    return _orientation;
}

void EntityModel::setOrientation( const EntityOrientationModel& orientation ) {
    _orientation = orientation;
}

EntityPositionModel EntityModel::position() const {
    return _position;
}

void EntityModel::setPosition( const EntityPositionModel& position ) {
    _position = position;
}

EntitySizeModel EntityModel::size() const {
    return _size;
}

void EntityModel::setSize( const EntitySizeModel& size ) {
    _size = size;
}

} // namespace Engine
