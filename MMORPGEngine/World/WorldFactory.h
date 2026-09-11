#ifndef WORLDFACTORY_H
#define WORLDFACTORY_H

#include <memory>
#include <string>

#include <MMORPGEngine/World/WorldModel.h>

namespace Engine {

class WorldFactory {
public:
    static std::unique_ptr<WorldModel> createWorld( const std::string& worldPath );

    static void saveWorld( const std::string& worldPath, const WorldModel& world );

private:
    static void createFloor( const std::string& floorFile, WorldModel* world );
    static void saveFloor( const std::string& floorFile, const WorldModel& world );
};

} // namespace Engine

#endif // WORLDFACTORY_H
