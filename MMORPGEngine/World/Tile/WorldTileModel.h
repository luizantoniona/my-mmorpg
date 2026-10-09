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

    const TileModel* overlayModel() const;
    void setOverlayModel( const TileModel* overlayModel );

    uint32_t overlayType() const;
    void setOverlayType( uint32_t overlayType );

private:
    const TileModel* _tileModel;
    const TileModel* _overlayModel;
    uint32_t _tileType;
    uint32_t _overlayType;
};

} // namespace Engine

#endif // WORLDTILEMODEL_H
