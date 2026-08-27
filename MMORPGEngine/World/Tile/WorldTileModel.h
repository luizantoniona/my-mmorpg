#ifndef WORLDTILEMODEL_H
#define WORLDTILEMODEL_H

#include <cstdint>

namespace Engine {

class WorldTileModel {
public:
    WorldTileModel();

    const TileModel* tileModel() const;
    void setTileModel( TileModel* tileModel );

    uint32_t tileType() const;
    void setTileType( uint32_t tileType );

private:
    const TileModel* _tileModel;
    uint32_t _tileType;
};

} // namespace Engine

#endif // WORLDTILEMODEL_H
