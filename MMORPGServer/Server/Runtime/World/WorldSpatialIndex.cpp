#include "WorldSpatialIndex.h"

#include <algorithm>
#include <cstdlib>

#include <MMORPGEngine/World/WorldConstants.h>

namespace {

constexpr int FLOOR_BELOW = -1;
constexpr int FLOOR_ABOVE = 1;

std::vector<int> characterIds( const std::vector<const Server::ChunkRuntime*>& chunks ) {
    std::vector<int> result;

    for ( const Server::ChunkRuntime* chunk : chunks ) {
        for ( const Engine::CharacterModel* characterPtr : chunk->characters() ) {
            result.push_back( characterPtr->idCharacter() );
        }
    }

    return result;
}

} // namespace

namespace Server {

WorldSpatialIndex::WorldSpatialIndex( const std::map<int, std::unique_ptr<CharacterRuntime>>& characters, const std::map<int, std::unique_ptr<CreatureRuntime>>& creatures ) :
    _characters( characters ),
    _creatures( creatures ) {
}

void WorldSpatialIndex::addCharacter( Engine::CharacterModel* character ) {
    const ChunkCoordinate coordinate = chunkCoordinateFor( character->position() );

    chunkOrCreate( coordinate ).characters().push_back( character );
    observe( coordinate, true );
}

void WorldSpatialIndex::removeCharacter( Engine::CharacterModel* character ) {
    const ChunkCoordinate coordinate = chunkCoordinateFor( character->position() );

    std::erase( chunkOrCreate( coordinate ).characters(), character );
    observe( coordinate, false );
}

void WorldSpatialIndex::updateCharacterChunk( Engine::CharacterModel* character, const Engine::EntityPositionModel& previousPosition ) {
    const ChunkCoordinate previousChunk = chunkCoordinateFor( previousPosition );
    const ChunkCoordinate newChunk = chunkCoordinateFor( character->position() );

    if ( previousChunk == newChunk ) {
        return;
    }

    std::erase( chunkOrCreate( previousChunk ).characters(), character );
    chunkOrCreate( newChunk ).characters().push_back( character );

    observe( newChunk, true );
    observe( previousChunk, false );
}

void WorldSpatialIndex::addCreature( CreatureRuntime* creature ) {
    chunkOrCreate( chunkCoordinateFor( creature->creature()->position() ) ).creatures().push_back( creature );
}

void WorldSpatialIndex::removeCreature( CreatureRuntime* creature ) {
    std::erase( chunkOrCreate( chunkCoordinateFor( creature->creature()->position() ) ).creatures(), creature );
}

void WorldSpatialIndex::updateCreatureChunk( CreatureRuntime* creature, const Engine::EntityPositionModel& previousPosition ) {
    const ChunkCoordinate previousChunk = chunkCoordinateFor( previousPosition );
    const ChunkCoordinate newChunk = chunkCoordinateFor( creature->creature()->position() );

    if ( previousChunk == newChunk ) {
        return;
    }

    std::erase( chunkOrCreate( previousChunk ).creatures(), creature );
    chunkOrCreate( newChunk ).creatures().push_back( creature );
}

void WorldSpatialIndex::addGroundItem( int idGroundItem, const Engine::EntityPositionModel& position ) {
    chunkOrCreate( chunkCoordinateFor( position ) ).groundItems().push_back( idGroundItem );
}

void WorldSpatialIndex::removeGroundItem( int idGroundItem, const Engine::EntityPositionModel& position ) {
    std::erase( chunkOrCreate( chunkCoordinateFor( position ) ).groundItems(), idGroundItem );
}

void WorldSpatialIndex::addSpawnArea( WorldSpawnAreaRuntime* spawnArea ) {
    chunkOrCreate( ChunkCoordinate( spawnArea->area().x() / Engine::WorldConstants::CHUNK_SIZE, spawnArea->area().y() / Engine::WorldConstants::CHUNK_SIZE, spawnArea->z() ) ).spawnAreas().push_back( spawnArea );
}

ChunkCoordinate WorldSpatialIndex::chunkCoordinateFor( const Engine::EntityPositionModel& position ) const {
    return ChunkCoordinate( position.x() / Engine::WorldConstants::CHUNK_SIZE, position.y() / Engine::WorldConstants::CHUNK_SIZE, position.z() );
}

const ChunkRuntime* WorldSpatialIndex::findChunk( const ChunkCoordinate& coordinate ) const {
    const auto it = _chunks.find( coordinate );

    return it != _chunks.end() ? it->second.get() : nullptr;
}

std::vector<ChunkRuntime*> WorldSpatialIndex::activeChunks() const {
    std::vector<ChunkRuntime*> result;

    for ( const auto& entry : _chunks ) {
        if ( entry.second->isActive() ) {
            result.push_back( entry.second.get() );
        }
    }

    std::sort( result.begin(), result.end(), []( const ChunkRuntime* left, const ChunkRuntime* right ) {
        const ChunkCoordinate& a = left->coordinate();
        const ChunkCoordinate& b = right->coordinate();

        return a.z != b.z ? a.z < b.z : ( a.y != b.y ? a.y < b.y : a.x < b.x );
    } );

    return result;
}

std::vector<int> WorldSpatialIndex::charactersVisibleFrom( int idCharacter ) const {
    auto it = _characters.find( idCharacter );
    if ( it == _characters.end() ) {
        return {};
    }

    std::vector<int> result = charactersVisibleFrom( it->second->character()->position() );

    std::erase( result, idCharacter );

    return result;
}

std::vector<int> WorldSpatialIndex::charactersVisibleFrom( const Engine::EntityPositionModel& position ) const {
    return characterIds( chunksAround( position, FLOOR_BELOW ) );
}

std::vector<int> WorldSpatialIndex::charactersViewing( int idCharacter ) const {
    auto it = _characters.find( idCharacter );
    if ( it == _characters.end() ) {
        return {};
    }

    std::vector<int> result = charactersViewing( it->second->character()->position() );

    std::erase( result, idCharacter );

    return result;
}

std::vector<int> WorldSpatialIndex::charactersViewing( const Engine::EntityPositionModel& position ) const {
    return characterIds( chunksAround( position, FLOOR_ABOVE ) );
}

Engine::CharacterModel* WorldSpatialIndex::characterAt( int x, int y, int z ) const {
    for ( const auto& entry : _characters ) {
        Engine::CharacterModel* characterPtr = entry.second->character();

        if ( characterPtr->occupies( x, y, z ) ) {
            return characterPtr;
        }
    }

    return nullptr;
}

Engine::CharacterModel* WorldSpatialIndex::nearestCharacterWithin( const Engine::EntityPositionModel& position, int radius ) const {
    Engine::CharacterModel* nearest = nullptr;
    int nearestDistance = radius + 1;

    for ( const auto& entry : _characters ) {
        Engine::CharacterModel* characterPtr = entry.second->character();
        const Engine::EntityPositionModel& characterPosition = characterPtr->position();

        if ( characterPosition.z() != position.z() ) {
            continue;
        }

        const int distance = std::max( std::abs( characterPosition.x() - position.x() ), std::abs( characterPosition.y() - position.y() ) );

        if ( distance < nearestDistance ) {
            nearest = characterPtr;
            nearestDistance = distance;
        }
    }

    return nearest;
}

std::vector<Engine::CreatureModel> WorldSpatialIndex::creaturesVisibleFrom( const Engine::EntityPositionModel& position ) const {
    std::vector<Engine::CreatureModel> result;

    for ( const ChunkRuntime* chunk : chunksAround( position, FLOOR_BELOW ) ) {
        for ( const CreatureRuntime* creatureRuntime : chunk->creatures() ) {
            result.push_back( *creatureRuntime->creature() );
        }
    }

    std::sort( result.begin(), result.end(), []( const Engine::CreatureModel& left, const Engine::CreatureModel& right ) {
        return left.idCreature() < right.idCreature();
    } );

    return result;
}

Engine::CreatureModel* WorldSpatialIndex::creatureAt( int x, int y, int z ) const {
    for ( const auto& entry : _creatures ) {
        Engine::CreatureModel* creaturePtr = entry.second->creature();

        if ( creaturePtr->occupies( x, y, z ) ) {
            return creaturePtr;
        }
    }

    return nullptr;
}

std::vector<int> WorldSpatialIndex::groundItemsVisibleFrom( const Engine::EntityPositionModel& position ) const {
    std::vector<int> result;

    for ( const ChunkRuntime* chunk : chunksAround( position, FLOOR_BELOW ) ) {
        result.insert( result.end(), chunk->groundItems().begin(), chunk->groundItems().end() );
    }

    std::sort( result.begin(), result.end() );

    return result;
}

bool WorldSpatialIndex::isPositionOccupied( const Engine::EntityPositionModel& position ) const {
    return isAreaOccupied( position.x(), position.y(), position.z(), 1, 1 );
}

bool WorldSpatialIndex::isAreaOccupied( int x, int y, int z, int width, int height, int ignoredIdCreature ) const {
    for ( const auto& entry : _characters ) {
        if ( entry.second->character()->overlaps( x, y, z, width, height ) ) {
            return true;
        }
    }

    for ( const auto& entry : _creatures ) {
        const Engine::CreatureModel* creaturePtr = entry.second->creature();

        if ( creaturePtr->idCreature() != ignoredIdCreature && creaturePtr->overlaps( x, y, z, width, height ) ) {
            return true;
        }
    }

    return false;
}

ChunkRuntime& WorldSpatialIndex::chunkOrCreate( const ChunkCoordinate& coordinate ) {
    std::unique_ptr<ChunkRuntime>& chunk = _chunks[ coordinate ];

    if ( !chunk ) {
        chunk = std::make_unique<ChunkRuntime>( coordinate );
    }

    return *chunk;
}

std::vector<const ChunkRuntime*> WorldSpatialIndex::chunksAround( const Engine::EntityPositionModel& position, int floorOffset ) const {
    const ChunkCoordinate center = chunkCoordinateFor( position );

    std::vector<const ChunkRuntime*> result;

    for ( int dz : { 0, floorOffset } ) {
        for ( int dx = -1; dx <= 1; ++dx ) {
            for ( int dy = -1; dy <= 1; ++dy ) {
                const ChunkRuntime* chunk = findChunk( ChunkCoordinate( center.x + dx, center.y + dy, center.z + dz ) );

                if ( chunk ) {
                    result.push_back( chunk );
                }
            }
        }
    }

    return result;
}

void WorldSpatialIndex::observe( const ChunkCoordinate& center, bool observed ) {
    for ( int dz : { 0, FLOOR_BELOW } ) {
        for ( int dx = -1; dx <= 1; ++dx ) {
            for ( int dy = -1; dy <= 1; ++dy ) {
                const int x = center.x + dx;
                const int y = center.y + dy;
                const int z = center.z + dz;

                if ( x < 0 || y < 0 || z < 0 ) {
                    continue;
                }

                ChunkRuntime& chunk = chunkOrCreate( ChunkCoordinate( x, y, z ) );

                if ( observed ) {
                    chunk.addObserver();
                } else {
                    chunk.removeObserver();
                }
            }
        }
    }
}

} // namespace Server
