#include "WorldTileModel.h"

namespace Engine {

WorldTileModel::WorldTileModel() :
    _tileModel( nullptr ),
    _overlayModel( nullptr ),
    _tileType( 0 ),
    _overlayType( 0 ) {
}

const TileModel* WorldTileModel::tileModel() const {
    return _tileModel;
}

void WorldTileModel::setTileModel( const TileModel* tileModel ) {
    _tileModel = tileModel;
}

uint32_t WorldTileModel::tileType() const {
    return _tileType;
}

void WorldTileModel::setTileType( uint32_t tileType ) {
    _tileType = tileType;
}

const TileModel* WorldTileModel::overlayModel() const {
    return _overlayModel;
}

void WorldTileModel::setOverlayModel( const TileModel* overlayModel ) {
    _overlayModel = overlayModel;
}

uint32_t WorldTileModel::overlayType() const {
    return _overlayType;
}

void WorldTileModel::setOverlayType( uint32_t overlayType ) {
    _overlayType = overlayType;
}

} // namespace Engine
