#include "WorldManager.h"

#include <chrono>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/World/WorldFactory.h>

namespace Server {

WorldManager::WorldManager() :
    _running( false ),
    _thread(),
    _runtime( nullptr ) {
}

WorldManager::~WorldManager() {
}

bool WorldManager::initialize( const std::string& worldPath ) {
    if ( !_runtime ) {
        const Json::Value configJson = Engine::JsonHelper::loadJsonFile( worldPath + "Config.json" );
        const Json::Value tickRateJson = configJson[ "Server" ][ "TickRate" ];

        if ( !tickRateJson.isInt() || tickRateJson.asInt() <= 0 ) {
            qCritical() << "[WorldManager] Invalid Server.TickRate in Config.json, expected a positive integer [VALUE]" << QString::fromStdString( Engine::JsonHelper::writeJsonString( tickRateJson ) );
            return false;
        }

        _runtime = std::make_unique<WorldRuntime>( Engine::WorldFactory::createWorld( worldPath ), tickRateJson.asInt() );
        _runtime->spawnCreaturesFromAreas();
    }

    if ( _running ) {
        return true;
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

    return true;
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
