#include "ObjectPositionModel.h"

namespace Engine {

ObjectPositionModel::ObjectPositionModel() :
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

ObjectPositionModel::~ObjectPositionModel() {
}

int ObjectPositionModel::x() const {
    return _x;
}

void ObjectPositionModel::setX( int x ) {
    _x = x;
}

int ObjectPositionModel::y() const {
    return _y;
}

void ObjectPositionModel::setY( int y ) {
    _y = y;
}

int ObjectPositionModel::z() const {
    return _z;
}

void ObjectPositionModel::setZ( int z ) {
    _z = z;
}

} // namespace Engine