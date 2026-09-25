#include "ObjectInteractionModel.h"

namespace Engine {

ObjectInteractionModel::ObjectInteractionModel() :
    _type( ObjectInteractionEnum::CONTAINER ),
    _containerCapacity( 0 ) {
}

ObjectInteractionEnum ObjectInteractionModel::type() const {
    return _type;
}

void ObjectInteractionModel::setType( ObjectInteractionEnum type ) {
    _type = type;
}

uint32_t ObjectInteractionModel::containerCapacity() const {
    return _containerCapacity;
}

void ObjectInteractionModel::setContainerCapacity( uint32_t containerCapacity ) {
    _containerCapacity = containerCapacity;
}

} // namespace Engine
