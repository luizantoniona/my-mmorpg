#include "EntityModel.h"

namespace Engine {

EntityModel::EntityModel() :
    _position(),
    _size(),
    _vitals() {
}

EntityModel::~EntityModel() = default;

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

EntityVitalsModel EntityModel::vitals() const {
    return _vitals;
}

void EntityModel::setVitals( const EntityVitalsModel& vitals ) {
    _vitals = vitals;
}

} // namespace Engine
