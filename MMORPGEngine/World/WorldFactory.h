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

    static bool addFloor( const std::string& worldPath, const WorldModel& world, bool above );
    static bool removeFloor( const std::string& worldPath, int z );

    static bool resizeWorld( const std::string& worldPath, uint32_t newWidth, uint32_t newHeight );
};

} // namespace Engine

#endif // WORLDFACTORY_H
