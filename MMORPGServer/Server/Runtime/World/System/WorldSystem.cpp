#include "WorldSystem.h"

namespace Server {

WorldSystem::WorldSystem( WorldRuntime& runtime ) :
    _runtime( runtime ) {
}

WorldSystem::~WorldSystem() = default;

} // namespace Server
