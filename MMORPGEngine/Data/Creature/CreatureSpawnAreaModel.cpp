#include "CreatureSpawnAreaModel.h"

namespace Engine {

CreatureSpawnAreaModel::CreatureSpawnAreaModel() :
    _creatures(),
    _x( 0 ),
    _y( 0 ),
    _width( 0 ),
    _height( 0 ) {
}

int CreatureSpawnAreaModel::x() const {
    return _x;
}

void CreatureSpawnAreaModel::setX( int x ) {
    _x = x;
}

int CreatureSpawnAreaModel::y() const {
    return _y;
}

void CreatureSpawnAreaModel::setY( int y ) {
    _y = y;
}

uint32_t CreatureSpawnAreaModel::width() const {
    return _width;
}

void CreatureSpawnAreaModel::setWidth( uint32_t width ) {
    _width = width;
}

uint32_t CreatureSpawnAreaModel::height() const {
    return _height;
}

void CreatureSpawnAreaModel::setHeight( uint32_t height ) {
    _height = height;
}

const std::vector<CreatureSpawnEntryModel>& CreatureSpawnAreaModel::creatures() const {
    return _creatures;
}

void CreatureSpawnAreaModel::setCreatures( const std::vector<CreatureSpawnEntryModel>& creatures ) {
    _creatures = creatures;
}

} // namespace Engine
