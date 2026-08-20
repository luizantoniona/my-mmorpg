#include "TileModel.h"

namespace Engine {

TileModel::TileModel() :
    _tileTextureId( 0 ) {
}

uint32_t TileModel::tileTextureId() const {
    return _tileTextureId;
}

void TileModel::setTileTextureId( uint32_t tileTextureId ) {
    _tileTextureId = tileTextureId;
}

} // namespace Engine
