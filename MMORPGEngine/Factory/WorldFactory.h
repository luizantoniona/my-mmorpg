#ifndef WORLDFACTORY_H
#define WORLDFACTORY_H

#include <memory>
#include <string>

#include <MMORPGEngine/World/WorldModel.h>

namespace Engine {

class WorldFactory {
public:
    static std::unique_ptr<WorldModel> createWorld( const std::string& worldPath );

private:
    static void createGroundCatalog( const std::string& groundsFile, WorldModel* world );
    static void createFloor( const std::string& floorFile, WorldModel* world );
};

} // namespace Engine

#endif // WORLDFACTORY_H
