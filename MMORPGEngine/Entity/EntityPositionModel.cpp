#include "EntityPositionModel.h"

namespace Engine {

EntityPositionModel::EntityPositionModel() :
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

EntityPositionModel::~EntityPositionModel() {
}

int EntityPositionModel::x() const {
    return _x;
}

void EntityPositionModel::setX( int x ) {
    _x = x;
}

int EntityPositionModel::y() const {
    return _y;
}

void EntityPositionModel::setY( int y ) {
    _y = y;
}

int EntityPositionModel::z() const {
    return _z;
}

void EntityPositionModel::setZ( int z ) {
    _z = z;
}

} // namespace Engine