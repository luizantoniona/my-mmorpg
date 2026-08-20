#ifndef TILETEXTURECATALOG_H
#define TILETEXTURECATALOG_H

#include <unordered_map>

#include <MMORPGEngine/Data/Tile/TileTextureModel.h>

namespace Engine {

class TileTextureCatalog {
public:
    TileTextureCatalog();

    const TileTextureModel* texture( uint32_t type ) const;
    const std::unordered_map<uint32_t, TileTextureModel>& textures() const;
    void addTexture( const TileTextureModel& texture );

private:
    std::unordered_map<uint32_t, TileTextureModel> _textures;
};

} // namespace Engine

#endif // TILETEXTURECATALOG_H
