#include "WorldObjectModel.h"

namespace Engine {

WorldObjectModel::WorldObjectModel() :
    _objectModel( nullptr ),
    _objectType( 0 ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

const ObjectModel* WorldObjectModel::objectModel() const {
    return _objectModel;
}

void WorldObjectModel::setObjectModel( const ObjectModel* objectModel ) {
    _objectModel = objectModel;
}

uint32_t WorldObjectModel::objectType() const {
    return _objectType;
}

void WorldObjectModel::setObjectType( uint32_t objectType ) {
    _objectType = objectType;
}

int WorldObjectModel::x() const {
    return _x;
}

void WorldObjectModel::setX( int x ) {
    _x = x;
}

int WorldObjectModel::y() const {
    return _y;
}

void WorldObjectModel::setY( int y ) {
    _y = y;
}

int WorldObjectModel::z() const {
    return _z;
}

void WorldObjectModel::setZ( int z ) {
    _z = z;
}

} // namespace Engine
