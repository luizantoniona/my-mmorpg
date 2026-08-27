#include "WorldTileModel.h"

namespace Engine {

WorldTileModel::WorldTileModel() :
    _tileTextureId( 0 ) {
}

uint32_t WorldTileModel::tileTextureId() const {
    return _tileTextureId;
}

void WorldTileModel::setTileTextureId( uint32_t tileTextureId ) {
    _tileTextureId = tileTextureId;
}

} // namespace Engine
