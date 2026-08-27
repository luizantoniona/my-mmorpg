#ifndef WORLDTILEMODEL_H
#define WORLDTILEMODEL_H

#include <cstdint>

#include <MMORPGEngine/Data/Tile/TileModel.h>

namespace Engine {

class WorldTileModel {
public:
    WorldTileModel();

    const TileModel* tileModel() const;
    void setTileModel( const TileModel* tileModel );

    uint32_t tileType() const;
    void setTileType( uint32_t tileType );

private:
    const TileModel* _tileModel;
    uint32_t _tileType;
};

} // namespace Engine

#endif // WORLDTILEMODEL_H
