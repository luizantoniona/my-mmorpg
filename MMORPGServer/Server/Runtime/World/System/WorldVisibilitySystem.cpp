#include "WorldVisibilitySystem.h"

#include <algorithm>
#include <iterator>
#include <mutex>

#include <MMORPGEngine/World/WorldConstants.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace Server {

WorldVisibilitySystem::WorldVisibilitySystem( WorldRuntime& runtime ) :
    WorldSystem( runtime ) {
}

void WorldVisibilitySystem::onTick() {
}

WorldVisibilityDelta WorldVisibilitySystem::delta( const Engine::EntityPositionModel& previousPosition, const Engine::EntityPositionModel& currentPosition, int ignoredIdCharacter ) {
    std::lock_guard<std::mutex> lock( _runtime.mutex() );

    const WorldSpatialIndex& spatialIndex = _runtime.spatialIndex();

    const auto sorted = []( std::vector<int> ids ) {
        std::sort( ids.begin(), ids.end() );
        return ids;
    };

    const auto difference = [ &sorted ]( const std::vector<int>& from, const std::vector<int>& other ) {
        const std::vector<int> sortedFrom = sorted( from );
        const std::vector<int> sortedOther = sorted( other );

        std::vector<int> result;
        std::set_difference( sortedFrom.begin(), sortedFrom.end(), sortedOther.begin(), sortedOther.end(), std::back_inserter( result ) );

        return result;
    };

    const auto withoutIgnored = [ ignoredIdCharacter ]( std::vector<int> ids ) {
        ids.erase( std::remove( ids.begin(), ids.end(), ignoredIdCharacter ), ids.end() );
        return ids;
    };

    const auto creatureIds = [ &spatialIndex ]( const Engine::EntityPositionModel& position ) {
        std::vector<int> ids;
        for ( const Engine::CreatureModel& creature : spatialIndex.creaturesVisibleFrom( position ) ) {
            ids.push_back( creature.idCreature() );
        }
        return ids;
    };

    const auto groundItemIds = [ this ]( const Engine::EntityPositionModel& position ) {
        std::vector<int> ids;
        for ( const Engine::GroundItemModel& groundItem : _runtime.groundItemSystem().itemsVisibleFromLocked( position ) ) {
            ids.push_back( groundItem.idGroundItem() );
        }
        return ids;
    };

    const std::vector<int> previousCharacters = withoutIgnored( spatialIndex.charactersVisibleFrom( previousPosition ) );
    const std::vector<int> currentCharacters = withoutIgnored( spatialIndex.charactersVisibleFrom( currentPosition ) );
    const std::vector<int> previousViewers = withoutIgnored( spatialIndex.charactersViewing( previousPosition ) );
    const std::vector<int> currentViewers = withoutIgnored( spatialIndex.charactersViewing( currentPosition ) );
    const std::vector<int> previousCreatures = creatureIds( previousPosition );
    const std::vector<int> currentCreatures = creatureIds( currentPosition );
    const std::vector<int> previousGroundItems = groundItemIds( previousPosition );
    const std::vector<int> currentGroundItems = groundItemIds( currentPosition );

    WorldVisibilityDelta visibilityDelta;
    visibilityDelta.charactersEntered = difference( currentCharacters, previousCharacters );
    visibilityDelta.charactersLeft = difference( previousCharacters, currentCharacters );
    visibilityDelta.viewersLeft = difference( previousViewers, currentViewers );
    visibilityDelta.creaturesEntered = difference( currentCreatures, previousCreatures );
    visibilityDelta.creaturesLeft = difference( previousCreatures, currentCreatures );
    visibilityDelta.groundItemsEntered = difference( currentGroundItems, previousGroundItems );
    visibilityDelta.groundItemsLeft = difference( previousGroundItems, currentGroundItems );

    const std::vector<ChunkCoordinate> previousChunks = terrainChunksNear( previousPosition );
    const std::vector<ChunkCoordinate> currentChunks = terrainChunksNear( currentPosition );

    for ( const ChunkCoordinate& chunk : currentChunks ) {
        if ( std::find( previousChunks.begin(), previousChunks.end(), chunk ) == previousChunks.end() ) {
            visibilityDelta.terrainChunksEntered.push_back( chunk );
        }
    }

    for ( const ChunkCoordinate& chunk : previousChunks ) {
        if ( std::find( currentChunks.begin(), currentChunks.end(), chunk ) == currentChunks.end() ) {
            visibilityDelta.terrainChunksLeft.push_back( chunk );
        }
    }

    return visibilityDelta;
}

std::vector<ChunkCoordinate> WorldVisibilitySystem::terrainChunksNear( const Engine::EntityPositionModel& position ) const {
    std::vector<ChunkCoordinate> result;

    const Engine::WorldModel* world = _runtime.world();
    if ( !world ) {
        return result;
    }

    const int chunkCountX = ( static_cast<int>( world->width() ) + Engine::WorldConstants::CHUNK_SIZE - 1 ) / Engine::WorldConstants::CHUNK_SIZE;
    const int chunkCountY = ( static_cast<int>( world->height() ) + Engine::WorldConstants::CHUNK_SIZE - 1 ) / Engine::WorldConstants::CHUNK_SIZE;

    const int centerX = position.x() / Engine::WorldConstants::CHUNK_SIZE;
    const int centerY = position.y() / Engine::WorldConstants::CHUNK_SIZE;

    for ( int chunkX = centerX - 1; chunkX <= centerX + 1; ++chunkX ) {
        for ( int chunkY = centerY - 1; chunkY <= centerY + 1; ++chunkY ) {
            if ( chunkX >= 0 && chunkY >= 0 && chunkX < chunkCountX && chunkY < chunkCountY ) {
                result.push_back( ChunkCoordinate( chunkX, chunkY, 0 ) );
            }
        }
    }

    return result;
}

} // namespace Server
