#include "MonsterSpawnAreaModel.h"

namespace Engine {

MonsterSpawnAreaModel::MonsterSpawnAreaModel() :
    _monsters(),
    _x( 0 ),
    _y( 0 ),
    _width( 0 ),
    _height( 0 ) {
}

int MonsterSpawnAreaModel::x() const {
    return _x;
}

void MonsterSpawnAreaModel::setX( int x ) {
    _x = x;
}

int MonsterSpawnAreaModel::y() const {
    return _y;
}

void MonsterSpawnAreaModel::setY( int y ) {
    _y = y;
}

uint32_t MonsterSpawnAreaModel::width() const {
    return _width;
}

void MonsterSpawnAreaModel::setWidth( uint32_t width ) {
    _width = width;
}

uint32_t MonsterSpawnAreaModel::height() const {
    return _height;
}

void MonsterSpawnAreaModel::setHeight( uint32_t height ) {
    _height = height;
}

const std::vector<MonsterSpawnEntryModel>& MonsterSpawnAreaModel::monsters() const {
    return _monsters;
}

void MonsterSpawnAreaModel::setMonsters( const std::vector<MonsterSpawnEntryModel>& monsters ) {
    _monsters = monsters;
}

} // namespace Engine
