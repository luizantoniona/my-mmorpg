#include "WorldFactory.h"

#include <iostream>

#include <Engine/Commons/JsonHelper.h>

namespace Engine {

std::unique_ptr<WorldModel> WorldFactory::createWorld( const std::string& worldPath ) {
    std::cout << "WorldFactory::createWorld" << std::endl;

    std::unique_ptr<WorldModel> world = std::make_unique<WorldModel>();

    Json::Value worldJson = JsonHelper::loadJsonFile( worldPath + "Config.json" );

    return world;
}

} // namespace Engine
