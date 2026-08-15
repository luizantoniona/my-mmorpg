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
    world->setName( mapJson[ "Name" ].asString() );

    qInfo() << "WorldFactory::createWorld" << "[MAP_NAME]" << world->name();

    // --- Size
    world->setWidth( static_cast<uint16_t>( mapJson[ "Width" ].asUInt() ) );
    world->setHeight( static_cast<uint16_t>( mapJson[ "Height" ].asUInt() ) );

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

    GroundCatalog& groundCatalog = world->groundCatalog();

    const Json::Value& grounds = json[ "Grounds" ];

    for ( const Json::Value& groundJson : grounds ) {
        GroundModel ground;

        ground.setType( static_cast<uint16_t>( groundJson[ "Type" ].asUInt() ) );
        ground.setWalkable( groundJson[ "Walkable" ].asBool() );
        ground.setName( groundJson[ "Name" ].asString() );
        ground.setFolder( groundJson[ "TextureFolder" ].asString() );

        groundCatalog.addGround( ground );

        qInfo() << "WorldFactory::createGroundCatalog" << "LOADED:" << ground.type() << ground.name();
    }
}

void WorldFactory::createFloor( const std::string& floorFile, WorldModel* world ) {
    qInfo() << "WorldFactory::createFloor" << "[FLOOR_FILE_PATH]" << floorFile;

    if ( !world ) {
        qInfo() << "WorldFactory::createFloor" << "World is nullptr";
        return;
    }

    Json::Value floorJson = JsonHelper::loadJsonFile( floorFile );

    const int32_t z = floorJson[ "Z" ].asInt();
    const Json::Value& groundRows = floorJson[ "Ground" ];

    if ( !groundRows.isArray() || groundRows.empty() ) {
        qInfo() << "WorldFactory::createFloor" << "Floor has no ground matrix";
        return;
    }

    uint16_t width = static_cast<uint16_t>( groundRows[ 0 ].size() );
    uint16_t height = static_cast<uint16_t>( groundRows.size() );

    FloorModel floor;
    floor.setZ( z );
    floor.resize( width, height );

    const GroundCatalog& groundCatalog = world->groundCatalog();

    for ( uint16_t y = 0; y < height; ++y ) {
        const Json::Value& row = groundRows[ y ];

        for ( uint16_t x = 0; x < width; ++x ) {
            uint16_t groundId = static_cast<uint16_t>( row[ x ].asUInt() );

            if ( !groundCatalog.ground( groundId ) ) {
                groundId = 0;
            }

            if ( TileModel* tile = floor.tile( x, y ) ) {
                tile->setGroundId( groundId );
                // TODO: Tile setters here
            }
        }
    }

    world->floors()[ z ] = std::move( floor );
}

} // namespace Engine
