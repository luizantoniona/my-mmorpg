#include "WorldFactory.h"

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>

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

    // --- Catalogs
    qInfo() << "WorldFactory::createWorld" << "Creating catalogs";

    createGroundCatalog( mapPath + mapJson[ "Catalogs" ][ "Grounds" ].asString(), world.get() );

    // --- Floors
    for ( const Json::Value& floorFile : mapJson[ "Floors" ] ) {
        createFloor( mapPath + floorFile.asString(), world.get() );
    }

    return world;
}

void WorldFactory::createGroundCatalog( const std::string& groundsFile, WorldModel* world ) {
    qInfo() << "WorldFactory::createGroundCatalog" << "[GROUND_FILE_PATH]" << groundsFile;

    if ( !world ) {
        qInfo() << "WorldFactory::createGroundCatalog" << "World is nullptr";
        return;
    }

    Json::Value json = JsonHelper::loadJsonFile( groundsFile );

    // GroundCatalog& groundCatalog = world->groundCatalog();

    // const Json::Value& grounds = json[ "Grounds" ];

    // for ( const Json::Value& groundJson : grounds ) {
    //     GroundModel ground;

    //     ground.setType( static_cast<uint16_t>( groundJson[ "Type" ].asUInt() ) );
    //     ground.setWalkable( groundJson[ "Walkable" ].asBool() );
    //     ground.setName( groundJson[ "Name" ].asString() );
    //     ground.setFolder( groundJson[ "TextureFolder" ].asString() );

    //     groundCatalog.addGround( ground );

    //     qInfo() << "WorldFactory::createGroundCatalog" << "LOADED:" << ground.type() << ground.name();
    // }
}

void WorldFactory::createFloor( const std::string& floorFile, WorldModel* world ) {
    qInfo() << "WorldFactory::createFloor" << "[FLOOR_FILE_PATH]" << floorFile;

    if ( !world ) {
        qInfo() << "WorldFactory::createFloor" << "World is nullptr";
        return;
    }

    Json::Value floorJson = JsonHelper::loadJsonFile( floorFile );

    const int z = floorJson[ "Z" ].asInt();

    const Json::Value& groundRows = floorJson[ "Ground" ];

    if ( !groundRows.isArray() || groundRows.empty() ) {
        qInfo() << "WorldFactory::createFloor" << "Floor has no ground matrix";
        return;
    }

    const int height = static_cast<int>( groundRows.size() );

    const int width = static_cast<int>( groundRows[ 0 ].size() );

    for ( int y = 0; y < height; ++y ) {
        const Json::Value& row = groundRows[ y ];

        for ( int x = 0; x < width; ++x ) {
            const uint16_t groundId = static_cast<uint16_t>( row[ x ].asUInt() );

            const int chunkX = x / ChunkModel::CHUNK_WIDTH;

            const int chunkY = y / ChunkModel::CHUNK_HEIGHT;

            const int localX = x % ChunkModel::CHUNK_WIDTH;

            const int localY = y % ChunkModel::CHUNK_HEIGHT;

            ChunkModel* chunk = world->chunk( chunkX, chunkY );

            if ( !chunk ) {
                continue;
            }

            TileModel* tile = chunk->tile( localX, localY, z );

            if ( !tile ) {
                continue;
            }

            tile->setGroundId( groundId );
        }
    }
}

} // namespace Engine
