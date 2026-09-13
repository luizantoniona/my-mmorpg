#include "FloorFactory.h"

#include <fstream>
#include <functional>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/World/WorldConstants.h>

namespace {

// TODO: See if we can remove this method
std::string buildRow( uint32_t width, const std::function<uint32_t( uint32_t )>& valueAt ) {
    std::string row = "[";

    for ( uint32_t x = 0; x < width; ++x ) {
        if ( x > 0 ) {
            row += ", ";
        }

        row += std::to_string( valueAt( x ) );
    }

    row += "]";

    return row;
}

} // namespace

namespace Engine {

void FloorFactory::createFloor( const std::string& floorFile, WorldModel* world ) {
    qInfo() << "FloorFactory::createFloor"
            << "[FLOOR_FILE_PATH]" << floorFile;

    if ( !world ) {
        qInfo() << "FloorFactory::createFloor"
                << "World is nullptr";
        return;
    }

    Json::Value floorJson = JsonHelper::loadJsonFile( floorFile );

    const int z = floorJson[ "Z" ].asInt();

    world->addFloor( z );

    // --- Tiles
    qInfo() << "FloorFactory::createFloor"
            << "Creating Tiles";
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
                    qWarning() << "FloorFactory::createFloor"
                               << "Unknown tile type:" << tileType << "at x y z:" << x << y << z;
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
    qInfo() << "FloorFactory::createFloor"
            << "Creating Objects";
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
                    qWarning() << "FloorFactory::createFloor"
                               << "Unknown object type:" << objectType << "at x y z:" << x << y << z;
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

// TODO: See if we really need to indent the file
void FloorFactory::saveFloor( const std::string& floorFile, const WorldModel& world ) {
    qInfo() << "FloorFactory::saveFloor"
            << "[FLOOR_FILE_PATH]" << floorFile;

    Json::Value existingFloorJson = JsonHelper::loadJsonFile( floorFile );
    const int z = existingFloorJson[ "Z" ].asInt();

    const uint32_t width = world.width();
    const uint32_t height = world.height();

    std::ofstream file( floorFile );
    if ( !file.is_open() ) {
        qWarning() << "FloorFactory::saveFloor"
                   << "Could not open file for writing:" << floorFile;
        return;
    }

    file << "{\n";
    file << "    \"Z\": " << z << ",\n";

    file << "    \"Tiles\": [\n";
    for ( uint32_t y = 0; y < height; ++y ) {
        file << "        " << buildRow( width, [ &world, y, z ]( uint32_t x ) {
            const WorldTileModel* tile = world.tile( static_cast<int>( x ), static_cast<int>( y ), z );
            return tile ? tile->tileType() : 0u;
        } );
        file << ( y + 1 < height ? ",\n" : "\n" );
    }
    file << "    ],\n";

    file << "    \"Objects\": [\n";
    for ( uint32_t y = 0; y < height; ++y ) {
        file << "        " << buildRow( width, [ &world, y, z ]( uint32_t x ) {
            const WorldObjectModel* object = world.object( static_cast<int>( x ), static_cast<int>( y ), z );
            return object ? object->objectType() : 0u;
        } );
        file << ( y + 1 < height ? ",\n" : "\n" );
    }
    file << "    ]\n";

    file << "}\n";
}

bool FloorFactory::createEmptyFloor( const std::string& floorFile, int z, uint32_t width, uint32_t height ) {
    qInfo() << "FloorFactory::createEmptyFloor"
            << "[FLOOR_FILE_PATH]" << floorFile;

    std::ofstream file( floorFile );
    if ( !file.is_open() ) {
        qWarning() << "FloorFactory::createEmptyFloor"
                   << "Could not open file for writing:" << floorFile;
        return false;
    }

    file << "{\n";
    file << "    \"Z\": " << z << ",\n";

    file << "    \"Tiles\": [\n";
    for ( uint32_t y = 0; y < height; ++y ) {
        file << "        " << buildRow( width, []( uint32_t ) { return 0u; } );
        file << ( y + 1 < height ? ",\n" : "\n" );
    }
    file << "    ],\n";

    file << "    \"Objects\": [\n";
    for ( uint32_t y = 0; y < height; ++y ) {
        file << "        " << buildRow( width, []( uint32_t ) { return 0u; } );
        file << ( y + 1 < height ? ",\n" : "\n" );
    }
    file << "    ]\n";

    file << "}\n";

    return true;
}

} // namespace Engine
