#include "WorldModel.h"

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Object/ObjectCatalog.h>
#include <MMORPGEngine/Data/Object/ObjectModel.h>
#include <MMORPGEngine/Data/Tile/TileCatalog.h>
#include <MMORPGEngine/Data/Tile/TileModel.h>
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

void WorldModel::setObject( int x, int y, int z, uint32_t objectType ) {
    const int chunkX = x / WorldConstants::CHUNK_SIZE;
    const int chunkY = y / WorldConstants::CHUNK_SIZE;
    const int localX = x % WorldConstants::CHUNK_SIZE;
    const int localY = y % WorldConstants::CHUNK_SIZE;

    ChunkModel* chunkModel = chunk( chunkX, chunkY );

    if ( objectType == 0 ) {
        chunkModel->setObject( localX, localY, z, nullptr );
        return;
    }

    const ObjectCatalog& catalog = Singleton<DataManager>::instance().objectCatalog();
    const ObjectModel* objectModel = catalog.object( objectType );

    if ( !objectModel ) {
        return;
    }

    WorldObjectModel* worldObject = chunkModel->object( localX, localY, z );

    worldObject->setObjectModel( objectModel );
    worldObject->setObjectType( objectType );
    worldObject->setX( x );
    worldObject->setY( y );
    worldObject->setZ( z );
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

void WorldModel::setTile( int x, int y, int z, uint32_t tileType ) {
    const TileCatalog& catalog = Singleton<DataManager>::instance().tileCatalog();
    const TileModel* tileModel = catalog.tile( tileType );

    if ( !tileModel ) {
        return;
    }

    const int chunkX = x / WorldConstants::CHUNK_SIZE;
    const int chunkY = y / WorldConstants::CHUNK_SIZE;
    const int localX = x % WorldConstants::CHUNK_SIZE;
    const int localY = y % WorldConstants::CHUNK_SIZE;

    ChunkModel* chunkModel = chunk( chunkX, chunkY );
    WorldTileModel* worldTile = chunkModel->tile( localX, localY, z );

    worldTile->setTileModel( tileModel );
    worldTile->setTileType( tileType );
}

QString WorldModel::chunkKey( int x, int y ) const {
    return QStringLiteral( "%1:%2" ).arg( x ).arg( y );
}

} // namespace Engine
