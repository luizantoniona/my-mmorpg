#include "ChunkModel.h"

namespace Engine {

ChunkModel::ChunkModel() {
}

ChunkModel::~ChunkModel() = default;

void ChunkModel::setObject( int x, int y, int z, std::unique_ptr<WorldObjectModel> object ) {
    if ( !validPosition( x, y ) ) {
        return;
    }

    auto iterator = _objects.find( z );

    if ( iterator == _objects.end() ) {
        iterator = _objects.emplace( z, std::vector<std::unique_ptr<WorldObjectModel>>( CHUNK_WIDTH * CHUNK_HEIGHT ) ).first;
    }

    iterator->second[ index( x, y ) ] = std::move( object );
}

WorldObjectModel* ChunkModel::object( int x, int y, int z ) {
    if ( !validPosition( x, y ) ) {
        return nullptr;
    }

    auto iterator = _objects.find( z );

    if ( iterator == _objects.end() ) {
        iterator = _objects.emplace( z, std::vector<std::unique_ptr<WorldObjectModel>>( CHUNK_WIDTH * CHUNK_HEIGHT ) ).first;
    }

    const int position = index( x, y );

    if ( !iterator->second[ position ] ) {
        iterator->second[ position ] = std::make_unique<WorldObjectModel>();
    }

    return iterator->second[ position ].get();
}

const WorldObjectModel* ChunkModel::object( int x, int y, int z ) const {

    if ( !validPosition( x, y ) ) {
        return nullptr;
    }

    const auto iterator = _objects.find( z );

    if ( iterator == _objects.end() ) {
        return nullptr;
    }

    return iterator->second[ index( x, y ) ].get();
}

void ChunkModel::setTile( int x, int y, int z, std::unique_ptr<WorldTileModel> tile ) {
    if ( !validPosition( x, y ) ) {
        return;
    }

    auto iterator = _tiles.find( z );

    if ( iterator == _tiles.end() ) {
        iterator = _tiles.emplace( z, std::vector<std::unique_ptr<WorldTileModel>>( CHUNK_WIDTH * CHUNK_HEIGHT ) ).first;
    }

    iterator->second[ index( x, y ) ] = std::move( tile );
}

WorldTileModel* ChunkModel::tile( int x, int y, int z ) {
    if ( !validPosition( x, y ) ) {
        return nullptr;
    }

    auto iterator = _tiles.find( z );

    if ( iterator == _tiles.end() ) {
        iterator = _tiles.emplace( z, std::vector<std::unique_ptr<WorldTileModel>>( CHUNK_WIDTH * CHUNK_HEIGHT ) ).first;
    }

    const int position = index( x, y );

    if ( !iterator->second[ position ] ) {
        iterator->second[ position ] = std::make_unique<WorldTileModel>();
    }

    return iterator->second[ position ].get();
}

const WorldTileModel* ChunkModel::tile( int x, int y, int z ) const {
    if ( !validPosition( x, y ) ) {
        return nullptr;
    }

    const auto iterator = _tiles.find( z );

    if ( iterator == _tiles.end() ) {
        return nullptr;
    }

    return iterator->second[ index( x, y ) ].get();
}

bool ChunkModel::validPosition( int x, int y ) const {
    return x >= 0 && x < CHUNK_WIDTH && y >= 0 && y < CHUNK_HEIGHT;
}

int ChunkModel::index( int x, int y ) const {
    return y * CHUNK_WIDTH + x;
}

} // namespace Engine
