#include "ChunkModel.h"

namespace Engine {

ChunkModel::ChunkModel() {
}

void ChunkModel::setTile( int x, int y, int z, std::unique_ptr<TileModel> tile ) {
    if ( x < 0 || x >= CHUNK_WIDTH ) {
        return;
    }

    if ( y < 0 || y >= CHUNK_HEIGHT ) {
        return;
    }

    auto iterator = _tiles.find( z );

    if ( iterator == _tiles.end() ) {
        iterator = _tiles.emplace( z, std::vector<std::unique_ptr<TileModel>>( CHUNK_WIDTH * CHUNK_HEIGHT ) ).first;
    }

    const int index = y * CHUNK_WIDTH + x;

    iterator->second[ index ] = std::move( tile );
}

TileModel* ChunkModel::tile( int x, int y, int z ) {
    if ( x < 0 || x >= CHUNK_WIDTH ) {
        return nullptr;
    }

    if ( y < 0 || y >= CHUNK_HEIGHT ) {
        return nullptr;
    }

    auto iterator = _tiles.find( z );

    if ( iterator == _tiles.end() ) {
        iterator = _tiles.emplace( z, std::vector<std::unique_ptr<TileModel>>( CHUNK_WIDTH * CHUNK_HEIGHT ) ).first;
    }

    const int index = y * CHUNK_WIDTH + x;

    if ( !iterator->second[ index ] ) {
        iterator->second[ index ] = std::make_unique<TileModel>();
    }

    return iterator->second[ index ].get();
}

const TileModel* ChunkModel::tile( int x, int y, int z ) const {
    if ( x < 0 || x >= CHUNK_WIDTH ) {
        return nullptr;
    }

    if ( y < 0 || y >= CHUNK_HEIGHT ) {
        return nullptr;
    }

    const auto iterator = _tiles.find( z );

    if ( iterator == _tiles.end() ) {
        return nullptr;
    }

    const int index = y * CHUNK_WIDTH + x;

    return iterator->second[ index ].get();
}

} // namespace Engine
