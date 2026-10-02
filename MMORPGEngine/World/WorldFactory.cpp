#include "WorldFactory.h"

#include <algorithm>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/World/FloorFactory.h>

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

    // --- Spawn
    const int defaultSpawnX = static_cast<int>( world->width() / 2 );
    const int defaultSpawnY = static_cast<int>( world->height() / 2 );

    world->setSpawnX( mapJson.get( "SpawnX", defaultSpawnX ).asInt() );
    world->setSpawnY( mapJson.get( "SpawnY", defaultSpawnY ).asInt() );
    world->setSpawnZ( mapJson.get( "SpawnZ", 0 ).asInt() );

    // --- Floors
    for ( const Json::Value& floorFile : mapJson[ "Floors" ] ) {
        FloorFactory::createFloor( mapPath + "Floors/" + floorFile.asString(), world.get() );
    }

    return world;
}

void WorldFactory::saveWorld( const std::string& worldPath, const WorldModel& world ) {
    qInfo() << "WorldFactory::saveWorld";

    Json::Value configJson = JsonHelper::loadJsonFile( worldPath + "Config.json" );

    const std::string mapFolder = configJson[ "ActiveFolder" ].asString();

    const std::string mapPath = worldPath + mapFolder + "/";

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    // --- Floors
    for ( const Json::Value& floorFile : mapJson[ "Floors" ] ) {
        FloorFactory::saveFloor( mapPath + "Floors/" + floorFile.asString(), world );
    }
}

bool WorldFactory::addFloor( const std::string& worldPath, const WorldModel& world, bool above ) {
    qInfo() << "WorldFactory::addFloor";

    Json::Value configJson = JsonHelper::loadJsonFile( worldPath + "Config.json" );

    const std::string mapFolder = configJson[ "ActiveFolder" ].asString();

    const std::string mapPath = worldPath + mapFolder + "/";

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    const std::vector<int> floors = world.floors();

    int nextZ = 0;
    if ( !floors.empty() ) {
        const auto minMax = std::minmax_element( floors.begin(), floors.end() );
        nextZ = above ? *minMax.second + 1 : *minMax.first - 1;
    }

    const std::string floorFileName = std::to_string( nextZ ) + ".json";
    const std::string floorFile = mapPath + "Floors/" + floorFileName;

    if ( !FloorFactory::createEmptyFloor( floorFile, nextZ, world.width(), world.height() ) ) {
        return false;
    }

    mapJson[ "Floors" ].append( floorFileName );
    JsonHelper::saveJsonFile( mapPath + "Map.json", mapJson );

    return true;
}

bool WorldFactory::removeFloor( const std::string& worldPath, int z ) {
    qInfo() << "WorldFactory::removeFloor";

    Json::Value configJson = JsonHelper::loadJsonFile( worldPath + "Config.json" );

    const std::string mapFolder = configJson[ "ActiveFolder" ].asString();

    const std::string mapPath = worldPath + mapFolder + "/";

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    if ( mapJson[ "Floors" ].size() <= 1 ) {
        qWarning() << "WorldFactory::removeFloor" << "Cannot remove the last remaining floor.";
        return false;
    }

    const std::string floorFileName = std::to_string( z ) + ".json";

    Json::Value remainingFloors( Json::arrayValue );
    bool found = false;
    for ( const Json::Value& floorFile : mapJson[ "Floors" ] ) {
        if ( floorFile.asString() == floorFileName ) {
            found = true;
            continue;
        }

        remainingFloors.append( floorFile );
    }

    if ( !found ) {
        qWarning() << "WorldFactory::removeFloor" << "Floor not found:" << z;
        return false;
    }

    mapJson[ "Floors" ] = remainingFloors;
    JsonHelper::saveJsonFile( mapPath + "Map.json", mapJson );

    FloorFactory::deleteFloor( mapPath + "Floors/" + floorFileName );

    return true;
}

bool WorldFactory::resizeWorld( const std::string& worldPath, uint32_t newWidth, uint32_t newHeight ) {
    qInfo() << "WorldFactory::resizeWorld";

    std::unique_ptr<WorldModel> world = createWorld( worldPath );

    if ( newWidth < world->width() || newHeight < world->height() ) {
        qWarning() << "WorldFactory::resizeWorld" << "New size must be greater than or equal to the current size.";
        return false;
    }

    world->setWidth( newWidth );
    world->setHeight( newHeight );

    saveWorld( worldPath, *world );

    Json::Value configJson = JsonHelper::loadJsonFile( worldPath + "Config.json" );

    const std::string mapFolder = configJson[ "ActiveFolder" ].asString();

    const std::string mapPath = worldPath + mapFolder + "/";

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );
    mapJson[ "Width" ] = newWidth;
    mapJson[ "Height" ] = newHeight;

    JsonHelper::saveJsonFile( mapPath + "Map.json", mapJson );

    return true;
}

} // namespace Engine
