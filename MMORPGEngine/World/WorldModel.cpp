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
    _height( 0 ),
    _spawnX( 0 ),
    _spawnY( 0 ),
    _spawnZ( 0 ) {
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

int WorldModel::spawnX() const {
    return _spawnX;
}

void WorldModel::setSpawnX( int spawnX ) {
    _spawnX = spawnX;
}

int WorldModel::spawnY() const {
    return _spawnY;
}

void WorldModel::setSpawnY( int spawnY ) {
    _spawnY = spawnY;
}

int WorldModel::spawnZ() const {
    return _spawnZ;
}

void WorldModel::setSpawnZ( int spawnZ ) {
    _spawnZ = spawnZ;
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

std::vector<int> WorldModel::floors() const {
    return std::vector<int>( _floors.begin(), _floors.end() );
}

void WorldModel::addFloor( int z ) {
    _floors.insert( z );
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
    const int chunkX = x / WorldConstants::CHUNK_SIZE;
    const int chunkY = y / WorldConstants::CHUNK_SIZE;
    const int localX = x % WorldConstants::CHUNK_SIZE;
    const int localY = y % WorldConstants::CHUNK_SIZE;

    ChunkModel* chunkModel = chunk( chunkX, chunkY );

    if ( tileType == 0 ) {
        chunkModel->setTile( localX, localY, z, nullptr );
        return;
    }

    const TileCatalog& catalog = Singleton<DataManager>::instance().tileCatalog();
    const TileModel* tileModel = catalog.tile( tileType );

    if ( !tileModel ) {
        return;
    }

    WorldTileModel* worldTile = chunkModel->tile( localX, localY, z );

    worldTile->setTileModel( tileModel );
    worldTile->setTileType( tileType );
}

std::vector<MonsterSpawnAreaModel> WorldModel::spawnAreas( int z ) const {
    const auto iterator = _spawnAreas.find( z );

    if ( iterator == _spawnAreas.end() ) {
        return {};
    }

    return iterator->second;
}

void WorldModel::addSpawnArea( int z, const MonsterSpawnAreaModel& spawnArea ) {
    _spawnAreas[ z ].push_back( spawnArea );
}

const MonsterSpawnAreaModel* WorldModel::spawnAreaAt( int x, int y, int z ) const {
    const auto iterator = _spawnAreas.find( z );

    if ( iterator == _spawnAreas.end() ) {
        return nullptr;
    }

    for ( const MonsterSpawnAreaModel& area : iterator->second ) {
        if ( x >= area.x() && x < area.x() + static_cast<int>( area.width() ) && y >= area.y() && y < area.y() + static_cast<int>( area.height() ) ) {
            return &area;
        }
    }

    return nullptr;
}

bool WorldModel::removeSpawnArea( int x, int y, int z ) {
    auto iterator = _spawnAreas.find( z );

    if ( iterator == _spawnAreas.end() ) {
        return false;
    }

    std::vector<MonsterSpawnAreaModel>& areas = iterator->second;

    for ( auto it = areas.begin(); it != areas.end(); ++it ) {
        if ( x >= it->x() && x < it->x() + static_cast<int>( it->width() ) && y >= it->y() && y < it->y() + static_cast<int>( it->height() ) ) {
            areas.erase( it );
            return true;
        }
    }

    return false;
}

QString WorldModel::chunkKey( int x, int y ) const {
    return QStringLiteral( "%1:%2" ).arg( x ).arg( y );
}

} // namespace Engine
