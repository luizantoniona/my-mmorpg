#include "TileTextureCatalog.h"

namespace Engine {

TileTextureCatalog::TileTextureCatalog() :
    _textures() {
}

const TileTextureModel* TileTextureCatalog::texture( uint32_t type ) const {
    auto iterator = _textures.find( type );

    if ( iterator == _textures.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

const std::unordered_map<uint32_t, TileTextureModel>& TileTextureCatalog::textures() const {
    return _textures;
}

void TileTextureCatalog::addTexture( const TileTextureModel& texture ) {
    _textures.insert( { texture.type(), texture } );
}

} // namespace Engine
