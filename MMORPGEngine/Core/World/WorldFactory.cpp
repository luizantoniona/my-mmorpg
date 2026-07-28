#include "WorldFactory.h"

#include <iostream>

#include <MMORPGEngine/Commons/JsonHelper.h>

namespace Engine {

std::unique_ptr<WorldModel> WorldFactory::createWorld( const std::string& worldPath ) {
    std::cout << "WorldFactory::createWorld" << std::endl;

    std::unique_ptr<WorldModel> world = std::make_unique<WorldModel>();

    Json::Value configJson = JsonHelper::loadJsonFile( worldPath + "Config.json" );

    std::string mapFolder = configJson["ActiveFolder"].asString();

    for ( const Json::Value& file : configJson["ActiveMapFiles"] ) {
        std::string fileName = file.asString();

        Json::Value floorJson = JsonHelper::loadJsonFile( worldPath + mapFolder + "/" + fileName );

        // auto floor = createFloor( floorJson );

        // world->addFloor( std::move( floor ) );
    }

    return world;
}

} // namespace Engine
