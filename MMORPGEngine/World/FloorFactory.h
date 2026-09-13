#ifndef FLOORFACTORY_H
#define FLOORFACTORY_H

#include <cstdint>
#include <string>

#include <MMORPGEngine/World/WorldModel.h>

namespace Engine {

class FloorFactory {
public:
    static void createFloor( const std::string& floorFile, WorldModel* world );
    static void saveFloor( const std::string& floorFile, const WorldModel& world );

    static bool createEmptyFloor( const std::string& floorFile, int z, uint32_t width, uint32_t height );
};

} // namespace Engine

#endif // FLOORFACTORY_H
