#include "WorldModel.h"

namespace Engine {

WorldModel::WorldModel() :
    _name( "" ),
    _width( 0 ),
    _height( 0 ),
    _groundCatalog(),
    _floors() {
}

std::string WorldModel::name() const {
    return _name;
}

void WorldModel::setName( const std::string& name ) {
    _name = name;
}

uint16_t WorldModel::width() const {
    return _width;
}

void WorldModel::setWidth( uint16_t width ) {
    _width = width;
}

uint16_t WorldModel::height() const {
    return _height;
}

void WorldModel::setHeight( uint16_t height ) {
    _height = height;
}

const GroundCatalog& WorldModel::groundCatalog() const {
    return _groundCatalog;
}

GroundCatalog& WorldModel::groundCatalog() {
    return _groundCatalog;
}

const std::unordered_map<int32_t, FloorModel>& WorldModel::floors() const {
    return _floors;
}

std::unordered_map<int32_t, FloorModel>& WorldModel::floors() {
    return _floors;
}

FloorModel* WorldModel::floor( int32_t z ) {
    const auto iterator = _floors.find( z );

    if ( iterator == _floors.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

const FloorModel* WorldModel::floor( int32_t z ) const {
    const auto iterator = _floors.find( z );

    if ( iterator == _floors.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

} // namespace Engine
