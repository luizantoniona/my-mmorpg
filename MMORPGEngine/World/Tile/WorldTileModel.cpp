#include "WorldTileModel.h"

namespace Engine {

WorldTileModel::WorldTileModel() :
    _tileModel( nullptr ),
    _tileType( 0 ) {
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

} // namespace Engine
