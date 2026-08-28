#include "WorldModel.h"

#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

WorldModel::WorldModel() :
    _name( "" ),
    _width( 0 ),
    _height( 0 ) {
}

QString WorldModel::name() const {
    return _name;
}

void WorldModel::setName( const QString& name ) {
    _name = name;
}

uint32_t WorldModel::width() const {
    return _width;
}

void WorldModel::setWidth( uint32_t width ) {
    _width = width;
}

uint32_t WorldModel::height() const {
    return _height;
}

void WorldModel::setHeight( uint32_t height ) {
    _height = height;
}

ChunkModel* WorldModel::chunk( int x, int y ) {
    const QString key = chunkKey( x, y );

    auto iterator = _chunks.find( key );

    if ( iterator == _chunks.end() ) {
        iterator = _chunks.emplace( key, std::make_unique<ChunkModel>() ).first;
    }

    return iterator->second.get();
}

const ChunkModel* WorldModel::chunk( int x, int y ) const {

    const auto iterator = _chunks.find( chunkKey( x, y ) );

    if ( iterator == _chunks.end() ) {
        return nullptr;
    }

    return iterator->second.get();
}

const WorldObjectModel* WorldModel::object( int x, int y, int z ) const {
    const int chunkX = x / WorldConstants::CHUNK_SIZE;
    const int chunkY = y / WorldConstants::CHUNK_SIZE;
    const int localX = x % WorldConstants::CHUNK_SIZE;
    const int localY = y % WorldConstants::CHUNK_SIZE;

    const auto iterator = _chunks.find( chunkKey( chunkX, chunkY ) );
    if ( iterator == _chunks.end() ) {
        return nullptr;
    }

    return iterator->second->object( localX, localY, z );
}

const WorldTileModel* WorldModel::tile( int x, int y, int z ) const {
    const int chunkX = x / WorldConstants::CHUNK_SIZE;
    const int chunkY = y / WorldConstants::CHUNK_SIZE;
    const int localX = x % WorldConstants::CHUNK_SIZE;
    const int localY = y % WorldConstants::CHUNK_SIZE;

    const auto iterator = _chunks.find( chunkKey( chunkX, chunkY ) );
    if ( iterator == _chunks.end() ) {
        return nullptr;
    }

    return iterator->second->tile( localX, localY, z );
}

QString WorldModel::chunkKey( int x, int y ) const {
    return QStringLiteral( "%1:%2" ).arg( x ).arg( y );
}

} // namespace Engine
