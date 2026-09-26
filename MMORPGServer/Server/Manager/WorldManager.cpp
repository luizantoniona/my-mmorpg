#include "WorldManager.h"

#include <chrono>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/World/WorldFactory.h>

namespace {

constexpr int DEFAULT_TICK_RATE = 20;

} // namespace

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
        const Json::Value configJson = Engine::JsonHelper::loadJsonFile( worldPath + "Config.json" );
        const int tickRate = configJson[ "Server" ].get( "TickRate", DEFAULT_TICK_RATE ).asInt();

        _runtime = std::make_unique<WorldRuntime>( Engine::WorldFactory::createWorld( worldPath ), tickRate );
        _runtime->spawnCreaturesFromAreas();
    }

    if ( _running ) {
        return;
    }

    const int msPerTick = 1000 / _runtime->tickRate();

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
