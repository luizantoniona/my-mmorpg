#include "TileCatalog.h"

namespace Engine {

TileCatalog::TileCatalog() :
    _tiles() {
}

const TileModel* TileCatalog::tile( uint32_t type ) const {
    auto iterator = _tiles.find( type );

    if ( iterator == _tiles.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

const std::unordered_map<uint32_t, TileModel>& TileCatalog::tiles() const {
    return _tiles;
}

void TileCatalog::addTile( const TileModel& tile ) {
    _tiles.insert( { tile.type(), tile } );
}

} // namespace Engine
