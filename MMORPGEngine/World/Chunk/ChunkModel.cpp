#include "ChunkModel.h"

namespace {
static constexpr int WIDTH = 32;
static constexpr int HEIGHT = 32;
} // namespace

namespace Engine {

ChunkModel::ChunkModel() {
}

void ChunkModel::setTile( int x, int y, int z, std::unique_ptr<TileModel> tile ) {
    if ( x < 0 || x >= WIDTH ) {
        return;
    }

    if ( y < 0 || y >= HEIGHT ) {
        return;
    }

    auto iterator = _tiles.find( z );

    if ( iterator == _tiles.end() ) {
        iterator = _tiles.insert( z, std::vector<std::unique_ptr<TileModel>>( WIDTH * HEIGHT ) );
    }

    const int index = y * WIDTH + x;

    iterator.value()[ index ] = std::move( tile );
}

const TileModel* ChunkModel::tile( int x, int y, int z ) const {
    if ( x < 0 || x >= WIDTH ) {
        return nullptr;
    }

    if ( y < 0 || y >= HEIGHT ) {
        return nullptr;
    }

    const auto iterator = _tiles.constFind( z );

    if ( iterator == _tiles.constEnd() ) {
        return nullptr;
    }

    const auto& tiles = iterator.value();

    const int index = y * WIDTH + x;

    return tiles[ index ].get();
}

} // namespace Engine
