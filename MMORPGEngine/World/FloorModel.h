#ifndef FLOORMODEL_H
#define FLOORMODEL_H

#include <cstdint>
#include <vector>

#include <MMORPGEngine/World/TileModel.h>

namespace Engine {

class FloorModel {
public:
    FloorModel();

    int32_t z() const;
    void setZ( int32_t z );

    uint16_t width() const;
    uint16_t height() const;

    void resize( uint16_t width, uint16_t height );

    TileModel* tile( uint16_t x, uint16_t y );
    const TileModel* tile( uint16_t x, uint16_t y ) const;

private:
    int32_t _z;
    uint16_t _width;
    uint16_t _height;
    std::vector<TileModel> _tiles;
};

} // namespace Engine

#endif // FLOORMODEL_H
