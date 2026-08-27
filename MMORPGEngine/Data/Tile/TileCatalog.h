#ifndef TILECATALOG_H
#define TILECATALOG_H

#include <unordered_map>

#include <MMORPGEngine/Data/Tile/TileModel.h>

namespace Engine {

class TileCatalog {
public:
    TileCatalog();

    const TileModel* tile( uint32_t type ) const;
    const std::unordered_map<uint32_t, TileModel>& tiles() const;
    void addTile( const TileModel& tile );

private:
    std::unordered_map<uint32_t, TileModel> _tiles;
};

} // namespace Engine

#endif // TILECATALOG_H
