#include "EntityPositionModel.h"

namespace Engine {

EntityPositionModel::EntityPositionModel() :
    _x( 0 ),
    _y( 0 ),
    _z( 0 ),
    _respawnX( 0 ),
    _respawnY( 0 ),
    _respawnZ( 0 ) {
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

int EntityPositionModel::respawnX() const {
    return _respawnX;
}

void EntityPositionModel::setRespawnX( int respawnX ) {
    _respawnX = respawnX;
}

int EntityPositionModel::respawnY() const {
    return _respawnY;
}

void EntityPositionModel::setRespawnY( int respawnY ) {
    _respawnY = respawnY;
}

int EntityPositionModel::respawnZ() const {
    return _respawnZ;
}

void EntityPositionModel::setRespawnZ( int respawnZ ) {
    _respawnZ = respawnZ;
}

} // namespace Engine
