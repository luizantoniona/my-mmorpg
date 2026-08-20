#include "ObjectModel.h"

namespace Engine {

ObjectModel::ObjectModel() :
    _objectTextureId( 0 ),
    _orientation(),
    _position(),
    _size() {
}

ObjectModel::~ObjectModel() = default;

uint32_t ObjectModel::objectTextureId() const {
    return _objectTextureId;
}

void ObjectModel::setObjectTextureId( uint32_t objectTextureId ) {
    _objectTextureId = objectTextureId;
}

ObjectOrientationModel ObjectModel::orientation() const {
    return _orientation;
}

void ObjectModel::setOrientation( const ObjectOrientationModel& orientation ) {
    _orientation = orientation;
}

ObjectPositionModel ObjectModel::position() const {
    return _position;
}

void ObjectModel::setPosition( const ObjectPositionModel& position ) {
    _position = position;
}

ObjectSizeModel ObjectModel::size() const {
    return _size;
}

void ObjectModel::setSize( const ObjectSizeModel& size ) {
    _size = size;
}

} // namespace Engine
