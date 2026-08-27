#include "WorldFactory.h"

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

std::unique_ptr<WorldModel> WorldFactory::createWorld( const std::string& worldPath ) {
    qInfo() << "WorldFactory::createWorld";

    std::unique_ptr<WorldModel> world = std::make_unique<WorldModel>();

    Json::Value configJson = JsonHelper::loadJsonFile( worldPath + "Config.json" );

    const std::string mapFolder = configJson[ "ActiveFolder" ].asString();

    const std::string mapPath = worldPath + mapFolder + "/";

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    // --- Name
    world->setName( QString( mapJson[ "Name" ].asCString() ) );

    qInfo() << "WorldFactory::createWorld" << "[MAP_NAME]" << world->name();

    // --- Size
    world->setWidth( mapJson[ "Width" ].asUInt() );
    world->setHeight( mapJson[ "Height" ].asUInt() );

    qInfo() << "WorldFactory::createWorld" << "[MAP_SIZE]" << world->width() << "x" << world->height();

    // --- Floors
    for ( const Json::Value& floorFile : mapJson[ "Floors" ] ) {
        createFloor( mapPath + "Floors/" + floorFile.asString(), world.get() );
    }

    return world;
}

void WorldFactory::createFloor( const std::string& floorFile, WorldModel* world ) {
    qInfo() << "WorldFactory::createFloor" << "[FLOOR_FILE_PATH]" << floorFile;

    if ( !world ) {
        qInfo() << "WorldFactory::createFloor" << "World is nullptr";
        return;
    }

    Json::Value floorJson = JsonHelper::loadJsonFile( floorFile );

    const int z = floorJson[ "Z" ].asInt();

    // -------------------------------------------------------------------------
    // Tiles
    // -------------------------------------------------------------------------
    qInfo() << "WorldFactory::createFloor" << "Creating Tiles";
    const Json::Value& tilesRows = floorJson[ "Tiles" ];
    if ( tilesRows.isArray() && !tilesRows.empty() ) {

        const TileCatalog& tileCatalog = Singleton<DataManager>::instance().tileCatalog();

        const int height = static_cast<int>( tilesRows.size() );
        const int width = static_cast<int>( tilesRows[ 0 ].size() );

        for ( int y = 0; y < height; ++y ) {
            const Json::Value& row = tilesRows[ y ];
            for ( int x = 0; x < width; ++x ) {

                const uint32_t tileType = row[ x ].asUInt();

                const TileModel* tileModel = tileCatalog.tile( tileType );

                if ( !tileModel ) {
                    qWarning() << "WorldFactory::createFloor" << "Unknown tile type:" << tileType << "at x y z:" << x << y << z;
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

    // -------------------------------------------------------------------------
    // Objects
    // -------------------------------------------------------------------------
    qInfo() << "WorldFactory::createFloor" << "Creating Objects";
    const Json::Value& objectRows = floorJson[ "Objects" ];
    if ( objectRows.isArray() && !objectRows.empty() ) {

        const ObjectCatalog& objectCatalog = Singleton<DataManager>::instance().objectCatalog();

        const int height = static_cast<int>( objectRows.size() );
        const int width = static_cast<int>( objectRows[ 0 ].size() );

        for ( int y = 0; y < height; ++y ) {
            const Json::Value& row = objectRows[ y ];
            for ( int x = 0; x < width; ++x ) {

                const uint32_t objectType = row[ x ].asUInt();

                if ( objectType == 0 ) {
                    continue;
                }

                const ObjectModel* objectModel = objectCatalog.object( objectType );

                if ( !objectModel ) {
                    qWarning() << "WorldFactory::createFloor" << "Unknown object type:" << objectType << "at x y z:" << x << y << z;
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
    }
}

} // namespace Engine
