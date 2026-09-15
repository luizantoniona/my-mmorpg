#include "WorldManager.h"

#include <chrono>

#include <QDebug>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/World/WorldFactory.h>

namespace Server {

WorldManager::WorldManager() :
    _running( false ),
    _thread(),
    _world( nullptr ) {
}

WorldManager::~WorldManager() {
}

void WorldManager::initialize( const std::string& worldPath ) {
    if ( !_world ) {
        _world = Engine::WorldFactory::createWorld( worldPath );
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

Engine::WorldModel* WorldManager::world() {
    return _world.get();
}

const Engine::WorldModel* WorldManager::world() const {
    return _world.get();
}

Engine::CharacterModel* WorldManager::addCharacter( std::unique_ptr<Engine::CharacterModel> character ) {
    std::lock_guard<std::mutex> lock( _mutex );

    const int idCharacter = character->idCharacter();
    Engine::CharacterModel* characterPtr = character.get();
    _characters[ idCharacter ] = std::move( character );

    qInfo() << "[WorldManager] Character added [CHARACTER]" << idCharacter << "[TOTAL]" << _characters.size();

    return characterPtr;
}

void WorldManager::removeCharacter( int idCharacter ) {
    std::lock_guard<std::mutex> lock( _mutex );

    _characters.erase( idCharacter );

    qInfo() << "[WorldManager] Character removed [CHARACTER]" << idCharacter << "[TOTAL]" << _characters.size();
}

Engine::CharacterModel* WorldManager::character( int idCharacter ) {
    std::lock_guard<std::mutex> lock( _mutex );

    auto it = _characters.find( idCharacter );
    return it != _characters.end() ? it->second.get() : nullptr;
}

std::map<int, Engine::EntityPositionModel> WorldManager::characterPositions() {
    std::lock_guard<std::mutex> lock( _mutex );

    std::map<int, Engine::EntityPositionModel> positions;
    for ( const auto& entry : _characters ) {
        positions[ entry.first ] = entry.second->position();
    }

    return positions;
}

} // namespace Server
