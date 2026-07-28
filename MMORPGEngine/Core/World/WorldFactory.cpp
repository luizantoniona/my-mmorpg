#include "WorldFactory.h"

#include <iostream>

#include <MMORPGEngine/Commons/JsonHelper.h>

namespace Engine {

std::unique_ptr<WorldModel> WorldFactory::createWorld( const std::string& worldPath ) {
    std::cout << "WorldFactory::createWorld" << std::endl;

    std::unique_ptr<WorldModel> world = std::make_unique<WorldModel>();

    Json::Value configJson = JsonHelper::loadJsonFile( worldPath + "Config.json" );

    const std::string mapFolder = configJson["ActiveFolder"].asString();

    const std::string mapPath = worldPath + mapFolder + "/";

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    // --- Name
    world->setName( mapJson["Name"].asString() );

    std::cout << "WorldFactory::createWorld" << "[MAP_NAME]" << world->name() << std::endl;

    // --- Size
    // world->setWidth( mapJson["Width"].asUInt() );
    // world->setHeight( mapJson["Height"].asUInt() );

    // --- Catalogs
    std::cout << "WorldFactory::createWorld" << "Creating catalogs" << std::endl;

    createGroundCatalog( mapPath + mapJson["Catalogs"]["Grounds"].asString(), world.get() );

    // --- Floors
    for ( const Json::Value& floorFile : mapJson["Floors"] ) {
        // createFloor( mapPath + floorFile.asString(), *world );
    }

    return world;
}

void WorldFactory::createGroundCatalog( const std::string& groundsFile, WorldModel* world ) {
    std::cout << "WorldFactory::createGroundCatalog" << "[GROUND_FILE_PATH]" << groundsFile << std::endl;

    if ( !world ) {
        std::cout << "WorldFactory::createGroundCatalog" << "World is nullptr" << std::endl;
        return;
    }

    Json::Value json = JsonHelper::loadJsonFile( groundsFile );

    GroundCatalog& groundCatalog = world->groundCatalog();

    const Json::Value& grounds = json["Grounds"];

    for ( const Json::Value& groundJson : grounds ) {
        GroundModel ground;

        ground.setType( static_cast<uint16_t>( groundJson["Type"].asUInt() ) );
        ground.setWalkable( groundJson["Walkable"].asBool() );
        ground.setName( groundJson["Name"].asString() );
        ground.setFolder( groundJson["TextureFolder"].asString() );

        groundCatalog.addGround( ground );

        std::cout << "WorldFactory::createGroundCatalog" << "LOADED:" << ground.type() << ground.name() << std::endl;
    }
}

} // namespace Engine
