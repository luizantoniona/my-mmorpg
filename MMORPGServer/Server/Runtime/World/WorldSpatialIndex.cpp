#include "WorldSpatialIndex.h"

#include <algorithm>
#include <cstdlib>

#include <MMORPGEngine/World/WorldConstants.h>

namespace Server {

WorldSpatialIndex::WorldSpatialIndex( const std::map<int, std::unique_ptr<CharacterRuntime>>& characters, const std::map<int, std::unique_ptr<CreatureRuntime>>& creatures ) :
    _characters( characters ),
    _creatures( creatures ) {
}

void WorldSpatialIndex::addCharacter( Engine::CharacterModel* character ) {
    _charactersByChunk[ chunkCoordinateFor( character->position() ) ].push_back( character );
}

void WorldSpatialIndex::removeCharacter( Engine::CharacterModel* character ) {
    auto& bucket = _charactersByChunk[ chunkCoordinateFor( character->position() ) ];
    bucket.erase( std::remove( bucket.begin(), bucket.end(), character ), bucket.end() );
}

void WorldSpatialIndex::updateCharacterChunk( Engine::CharacterModel* character, const Engine::EntityPositionModel& previousPosition ) {
    const ChunkCoordinate previousChunk = chunkCoordinateFor( previousPosition );
    const ChunkCoordinate newChunk = chunkCoordinateFor( character->position() );

    if ( previousChunk == newChunk ) {
        return;
    }

    auto& previousBucket = _charactersByChunk[ previousChunk ];
    previousBucket.erase( std::remove( previousBucket.begin(), previousBucket.end(), character ), previousBucket.end() );

    _charactersByChunk[ newChunk ].push_back( character );
}

ChunkCoordinate WorldSpatialIndex::chunkCoordinateFor( const Engine::EntityPositionModel& position ) const {
    return ChunkCoordinate( position.x() / Engine::WorldConstants::CHUNK_SIZE, position.y() / Engine::WorldConstants::CHUNK_SIZE, position.z() );
}

std::vector<int> WorldSpatialIndex::charactersNear( int idCharacter ) const {
    auto it = _characters.find( idCharacter );
    if ( it == _characters.end() ) {
        return {};
    }

    std::vector<int> result = charactersNear( it->second->character()->position() );

    result.erase( std::remove( result.begin(), result.end(), idCharacter ), result.end() );

    return result;
}

std::vector<int> WorldSpatialIndex::charactersNear( const Engine::EntityPositionModel& position ) const {
    std::vector<int> result;

    const ChunkCoordinate centerChunk = chunkCoordinateFor( position );

    for ( int dx = -1; dx <= 1; ++dx ) {
        for ( int dy = -1; dy <= 1; ++dy ) {
            const ChunkCoordinate neighborChunk( centerChunk.x + dx, centerChunk.y + dy, centerChunk.z );

            auto chunkIt = _charactersByChunk.find( neighborChunk );
            if ( chunkIt == _charactersByChunk.end() ) {
                continue;
            }

            for ( Engine::CharacterModel* characterPtr : chunkIt->second ) {
                result.push_back( characterPtr->idCharacter() );
            }
        }
    }

    return result;
}

bool WorldSpatialIndex::hasCharacterNear( const Engine::EntityPositionModel& position ) const {
    const ChunkCoordinate centerChunk = chunkCoordinateFor( position );

    for ( int dx = -1; dx <= 1; ++dx ) {
        for ( int dy = -1; dy <= 1; ++dy ) {
            const ChunkCoordinate neighborChunk( centerChunk.x + dx, centerChunk.y + dy, centerChunk.z );

            auto chunkIt = _charactersByChunk.find( neighborChunk );
            if ( chunkIt != _charactersByChunk.end() && !chunkIt->second.empty() ) {
                return true;
            }
        }
    }

    return false;
}

Engine::CharacterModel* WorldSpatialIndex::characterAt( int x, int y, int z ) const {
    for ( const auto& entry : _characters ) {
        Engine::CharacterModel* characterPtr = entry.second->character();
        const Engine::EntityPositionModel& position = characterPtr->position();

        if ( position.x() == x && position.y() == y && position.z() == z ) {
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

std::vector<Engine::CreatureModel> WorldSpatialIndex::creaturesNear( const Engine::EntityPositionModel& position ) const {
    const ChunkCoordinate centerChunk = chunkCoordinateFor( position );

    std::vector<Engine::CreatureModel> result;

    for ( const auto& entry : _creatures ) {
        const Engine::CreatureModel* creaturePtr = entry.second->creature();
        const ChunkCoordinate creatureChunk = chunkCoordinateFor( creaturePtr->position() );

        if ( creatureChunk.z != centerChunk.z ) {
            continue;
        }

        if ( std::abs( creatureChunk.x - centerChunk.x ) > 1 || std::abs( creatureChunk.y - centerChunk.y ) > 1 ) {
            continue;
        }

        result.push_back( *creaturePtr );
    }

    return result;
}

Engine::CreatureModel* WorldSpatialIndex::creatureAt( int x, int y, int z ) const {
    for ( const auto& entry : _creatures ) {
        Engine::CreatureModel* creaturePtr = entry.second->creature();
        const Engine::EntityPositionModel& position = creaturePtr->position();

        if ( position.x() == x && position.y() == y && position.z() == z ) {
            return creaturePtr;
        }
    }

    return nullptr;
}

bool WorldSpatialIndex::isPositionOccupied( const Engine::EntityPositionModel& position ) const {
    for ( const auto& entry : _characters ) {
        const Engine::EntityPositionModel& characterPosition = entry.second->character()->position();

        if ( characterPosition.x() == position.x() && characterPosition.y() == position.y() && characterPosition.z() == position.z() ) {
            return true;
        }
    }

    return creatureAt( position.x(), position.y(), position.z() ) != nullptr;
}

} // namespace Server
