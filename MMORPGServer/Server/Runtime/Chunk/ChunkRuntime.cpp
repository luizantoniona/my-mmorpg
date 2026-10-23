#include "ChunkRuntime.h"

#include <algorithm>

namespace Server {

ChunkRuntime::ChunkRuntime( const ChunkCoordinate& coordinate ) :
    _coordinate( coordinate ),
    _observerCount( 0 ) {
}

const ChunkCoordinate& ChunkRuntime::coordinate() const {
    return _coordinate;
}

std::vector<Engine::CharacterModel*>& ChunkRuntime::characters() {
    return _characters;
}

const std::vector<Engine::CharacterModel*>& ChunkRuntime::characters() const {
    return _characters;
}

std::vector<CreatureRuntime*>& ChunkRuntime::creatures() {
    return _creatures;
}

const std::vector<CreatureRuntime*>& ChunkRuntime::creatures() const {
    return _creatures;
}

std::vector<int>& ChunkRuntime::groundItems() {
    return _groundItems;
}

const std::vector<int>& ChunkRuntime::groundItems() const {
    return _groundItems;
}

std::vector<WorldSpawnAreaRuntime*>& ChunkRuntime::spawnAreas() {
    return _spawnAreas;
}

const std::vector<WorldSpawnAreaRuntime*>& ChunkRuntime::spawnAreas() const {
    return _spawnAreas;
}

void ChunkRuntime::addObserver() {
    ++_observerCount;
}

void ChunkRuntime::removeObserver() {
    if ( _observerCount > 0 ) {
        --_observerCount;
    }
}

bool ChunkRuntime::hasObservers() const {
    return _observerCount > 0;
}

bool ChunkRuntime::hasPendingRespawn() const {
    return std::any_of( _spawnAreas.begin(), _spawnAreas.end(), []( const WorldSpawnAreaRuntime* area ) {
        return area->hasPending();
    } );
}

bool ChunkRuntime::isActive() const {
    return hasObservers() || hasPendingRespawn();
}

} // namespace Server
