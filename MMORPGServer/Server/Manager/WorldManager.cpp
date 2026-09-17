#include "WorldManager.h"

#include <chrono>

#include <MMORPGEngine/World/WorldFactory.h>

namespace Server {

WorldManager::WorldManager() :
    _running( false ),
    _thread(),
    _runtime( nullptr ) {
}

WorldManager::~WorldManager() {
}

void WorldManager::initialize( const std::string& worldPath ) {
    if ( !_runtime ) {
        _runtime = std::make_unique<WorldRuntime>( Engine::WorldFactory::createWorld( worldPath ) );
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

            _runtime->tick();

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

WorldRuntime& WorldManager::runtime() {
    return *_runtime;
}

} // namespace Server
