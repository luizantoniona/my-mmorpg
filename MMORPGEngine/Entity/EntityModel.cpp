#include "EntityModel.h"

namespace Engine {

EntityModel::EntityModel() :
    _position() {
}

EntityModel::~EntityModel() = default;

EntityPositionModel EntityModel::position() const {
    return _position;
}

void EntityModel::setPosition( const EntityPositionModel& position ) {
    _position = position;
}

} // namespace Engine
