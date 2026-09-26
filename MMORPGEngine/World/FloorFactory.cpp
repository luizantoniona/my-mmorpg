#include "FloorFactory.h"

#include <cstdio>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Creature/CreatureSpawnAreaModel.h>
#include <MMORPGEngine/Data/Creature/CreatureSpawnEntryModel.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

void FloorFactory::createFloor( const std::string& floorFile, WorldModel* world ) {
    qInfo() << "FloorFactory::createFloor" << "[FLOOR_FILE_PATH]" << floorFile;

    if ( !world ) {
        qInfo() << "FloorFactory::createFloor" << "World is nullptr";
        return;
    }

    Json::Value floorJson = JsonHelper::loadJsonFile( floorFile );

    const int z = floorJson[ "Z" ].asInt();

    world->addFloor( z );

    // --- Tiles
    qInfo() << "FloorFactory::createFloor" << "Creating Tiles";

    const Json::Value& tilesRows = floorJson[ "Tiles" ];
    if ( tilesRows.isArray() && !tilesRows.empty() ) {

        const TileCatalog& tileCatalog = Singleton<DataManager>::instance().tileCatalog();

        const int height = static_cast<int>( tilesRows.size() );
        const int width = static_cast<int>( tilesRows[ 0 ].size() );

        for ( int y = 0; y < height; ++y ) {
            const Json::Value& row = tilesRows[ y ];
            for ( int x = 0; x < width; ++x ) {

                const uint32_t tileType = row[ x ].asUInt();

                if ( tileType == 0 ) {
                    continue;
                }

                const TileModel* tileModel = tileCatalog.tile( tileType );

                if ( !tileModel ) {
                    qWarning() << "FloorFactory::createFloor" << "Unknown tile type:" << tileType << "at x y z:" << x << y << z;
                    continue;
                }

                const int chunkX = x / WorldConstants::CHUNK_SIZE;
                const int chunkY = y / WorldConstants::CHUNK_SIZE;

                const int localX = x % WorldConstants::CHUNK_SIZE;
                const int localY = y % WorldConstants::CHUNK_SIZE;

                ChunkModel* chunk = world->chunk( chunkX, chunkY );
                if ( !chunk ) {
                    continue;
                }

                auto worldTile = std::make_unique<WorldTileModel>();
                worldTile->setTileModel( tileModel );
                worldTile->setTileType( tileType );

                chunk->setTile( localX, localY, z, std::move( worldTile ) );
            }
        }
    }

    // --- Objects
    qInfo() << "FloorFactory::createFloor" << "Creating Objects";

    const Json::Value& objects = floorJson[ "Objects" ];
    if ( objects.isArray() ) {

        const ObjectCatalog& objectCatalog = Singleton<DataManager>::instance().objectCatalog();

        for ( const Json::Value& objectJson : objects ) {

            if ( !objectJson.isMember( "X" ) || !objectJson.isMember( "Y" ) || !objectJson.isMember( "Type" ) ) {
                qWarning() << "FloorFactory::createFloor" << "Invalid Object entry, skipping";
                continue;
            }

            const int x = objectJson[ "X" ].asInt();
            const int y = objectJson[ "Y" ].asInt();
            const uint32_t objectType = objectJson[ "Type" ].asUInt();

            const ObjectModel* objectModel = objectCatalog.object( objectType );

            if ( !objectModel ) {
                qWarning() << "FloorFactory::createFloor" << "Unknown object type:" << objectType << "at x y z:" << x << y << z;
                continue;
            }

            const int chunkX = x / WorldConstants::CHUNK_SIZE;
            const int chunkY = y / WorldConstants::CHUNK_SIZE;

            const int localX = x % WorldConstants::CHUNK_SIZE;
            const int localY = y % WorldConstants::CHUNK_SIZE;

            ChunkModel* chunk = world->chunk( chunkX, chunkY );
            if ( !chunk ) {
                continue;
            }

            auto worldObject = std::make_unique<WorldObjectModel>();
            worldObject->setObjectModel( objectModel );
            worldObject->setObjectType( objectType );

            chunk->setObject( localX, localY, z, std::move( worldObject ) );
        }
    }

    // --- Spawn Areas
    qInfo() << "FloorFactory::createFloor" << "Creating Spawn Areas";

    const Json::Value& spawnAreas = floorJson[ "SpawnAreas" ];
    if ( spawnAreas.isArray() ) {

        const CreatureTypeCatalog& creatureTypeCatalog = Singleton<DataManager>::instance().creatureTypeCatalog();

        for ( const Json::Value& areaJson : spawnAreas ) {

            if ( !areaJson.isMember( "X" ) || !areaJson.isMember( "Y" ) || !areaJson.isMember( "Width" ) || !areaJson.isMember( "Height" ) ) {
                qWarning() << "FloorFactory::createFloor" << "Invalid SpawnArea entry, skipping";
                continue;
            }

            CreatureSpawnAreaModel area;
            area.setX( areaJson[ "X" ].asInt() );
            area.setY( areaJson[ "Y" ].asInt() );
            area.setWidth( areaJson[ "Width" ].asUInt() );
            area.setHeight( areaJson[ "Height" ].asUInt() );

            std::vector<CreatureSpawnEntryModel> entries;
            for ( const Json::Value& entryJson : areaJson[ "Creatures" ] ) {

                if ( !entryJson.isMember( "Type" ) || !entryJson.isMember( "Quantity" ) ) {
                    qWarning() << "FloorFactory::createFloor" << "Invalid SpawnArea Creature entry, skipping";
                    continue;
                }

                const uint32_t creatureType = entryJson[ "Type" ].asUInt();

                if ( !creatureTypeCatalog.creatureType( creatureType ) ) {
                    qWarning() << "FloorFactory::createFloor" << "Unknown creature type:" << creatureType << "in SpawnArea at x y z:" << area.x() << area.y() << z;
                    continue;
                }

                CreatureSpawnEntryModel entry;
                entry.setType( creatureType );
                entry.setQuantity( entryJson[ "Quantity" ].asUInt() );

                entries.push_back( entry );
            }
            area.setCreatures( entries );

            world->addSpawnArea( z, area );
        }
    }
}

void FloorFactory::saveFloor( const std::string& floorFile, const WorldModel& world ) {
    qInfo() << "FloorFactory::saveFloor" << "[FLOOR_FILE_PATH]" << floorFile;

    Json::Value existingFloorJson = JsonHelper::loadJsonFile( floorFile );
    const int z = existingFloorJson[ "Z" ].asInt();

    const uint32_t width = world.width();
    const uint32_t height = world.height();

    Json::Value floorJson;
    floorJson[ "Z" ] = z;

    Json::Value tilesJson( Json::arrayValue );
    for ( uint32_t y = 0; y < height; ++y ) {
        Json::Value rowJson( Json::arrayValue );
        for ( uint32_t x = 0; x < width; ++x ) {
            const WorldTileModel* tile = world.tile( static_cast<int>( x ), static_cast<int>( y ), z );
            rowJson.append( tile ? tile->tileType() : 0u );
        }
        tilesJson.append( rowJson );
    }
    floorJson[ "Tiles" ] = tilesJson;

    Json::Value objectsJson( Json::arrayValue );
    for ( uint32_t y = 0; y < height; ++y ) {
        for ( uint32_t x = 0; x < width; ++x ) {
            const WorldObjectModel* object = world.object( static_cast<int>( x ), static_cast<int>( y ), z );
            if ( !object || object->objectType() == 0 ) {
                continue;
            }

            Json::Value objectJson;
            objectJson[ "X" ] = x;
            objectJson[ "Y" ] = y;
            objectJson[ "Type" ] = object->objectType();

            objectsJson.append( objectJson );
        }
    }
    floorJson[ "Objects" ] = objectsJson;

    Json::Value spawnAreasJson( Json::arrayValue );
    for ( const CreatureSpawnAreaModel& area : world.spawnAreas( z ) ) {
        Json::Value areaJson;
        areaJson[ "X" ] = area.x();
        areaJson[ "Y" ] = area.y();
        areaJson[ "Width" ] = area.width();
        areaJson[ "Height" ] = area.height();

        Json::Value creaturesJson( Json::arrayValue );
        for ( const CreatureSpawnEntryModel& entry : area.creatures() ) {
            Json::Value entryJson;
            entryJson[ "Type" ] = entry.type();
            entryJson[ "Quantity" ] = entry.quantity();

            creaturesJson.append( entryJson );
        }
        areaJson[ "Creatures" ] = creaturesJson;

        spawnAreasJson.append( areaJson );
    }
    floorJson[ "SpawnAreas" ] = spawnAreasJson;

    JsonHelper::saveJsonFile( floorFile, floorJson );
}

bool FloorFactory::createEmptyFloor( const std::string& floorFile, int z, uint32_t width, uint32_t height ) {
    qInfo() << "FloorFactory::createEmptyFloor" << "[FLOOR_FILE_PATH]" << floorFile;

    Json::Value floorJson;
    floorJson[ "Z" ] = z;

    Json::Value tilesJson( Json::arrayValue );
    for ( uint32_t y = 0; y < height; ++y ) {
        Json::Value rowJson( Json::arrayValue );
        for ( uint32_t x = 0; x < width; ++x ) {
            rowJson.append( 0u );
        }
        tilesJson.append( rowJson );
    }
    floorJson[ "Tiles" ] = tilesJson;

    floorJson[ "Objects" ] = Json::Value( Json::arrayValue );
    floorJson[ "SpawnAreas" ] = Json::Value( Json::arrayValue );

    return JsonHelper::saveJsonFile( floorFile, floorJson );
}

void FloorFactory::deleteFloor( const std::string& floorFile ) {
    qInfo() << "FloorFactory::deleteFloor" << "[FLOOR_FILE_PATH]" << floorFile;

    std::remove( floorFile.c_str() );
}

} // namespace Engine
