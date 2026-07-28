#ifndef WORLDFACTORY_H
#define WORLDFACTORY_H

#include <memory>
#include <string>

#include <MMORPGEngine/Core/World/Model/WorldModel.h>

namespace Engine {

class WorldFactory {
public:
    static std::unique_ptr<WorldModel> createWorld( const std::string& worldPath );
};

} // namespace Engine

#endif // WORLDFACTORY_H
