#include "FloorModel.h"

namespace Engine {

FloorModel::FloorModel() :
    _z( 0 ),
    _width( 0 ),
    _height( 0 ),
    _tiles() {
}

int32_t FloorModel::z() const {
    return _z;
}

void FloorModel::setZ( int32_t z ) {
    _z = z;
}

uint16_t FloorModel::width() const {
    return _width;
}

uint16_t FloorModel::height() const {
    return _height;
}

void FloorModel::resize( uint16_t width, uint16_t height ) {
    _width = width;
    _height = height;
    _tiles.resize( static_cast<size_t>( width ) * height );
}

TileModel* FloorModel::tile( int32_t x, int32_t y ) {
    if ( x >= _width || y >= _height ) {
        return nullptr;
    }

    return &_tiles[ static_cast<size_t>( y ) * _width + x ];
}

const TileModel* FloorModel::tile( int32_t x, int32_t y ) const {
    if ( x >= _width || y >= _height ) {
        return nullptr;
    }

    return &_tiles[ static_cast<size_t>( y ) * _width + x ];
}

} // namespace Engine
