#include "WorldManager.h"

#include <chrono>

#include <Engine/Commons/Singleton.h>

#include "WorldFactory.h"

namespace Engine {

WorldManager::WorldManager() :
    _running( false ),
    _thread(),
    _world( nullptr ) {
}

WorldManager::~WorldManager() {}

void WorldManager::initialize( const std::string& worldPath ) {
    if ( !_world ) {
        _world = WorldFactory::createWorld( worldPath );
    }

    if ( _running ) {
        return;
    }

    // TODO: Create WorldConfigurationManager and store tickRate;
    const int tickRate = 20;
    const int msPerTick = 1000 / tickRate;

    _running = true;
    _thread = std::thread( [ this, msPerTick ]() {
        using clock = std::chrono::steady_clock;
        auto nextTick = clock::now();

        while ( _running ) {
            nextTick += std::chrono::milliseconds( msPerTick );

            // TODO: Tick runtime world and thread systems

            std::this_thread::sleep_until( nextTick );
        }
    } );
}

void WorldManager::finalize() {
    _running = false;

    if ( _thread.joinable() ) {
        _thread.join();
    }
}

} // namespace Engine
