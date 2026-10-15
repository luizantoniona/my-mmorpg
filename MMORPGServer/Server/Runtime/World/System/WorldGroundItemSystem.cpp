#include "WorldGroundItemSystem.h"

#include <algorithm>
#include <cstdlib>

#include <MMORPGEngine/World/WorldConstants.h>

namespace {

constexpr int PICKUP_REACH_TILES = 1;

} // namespace

namespace Server {

WorldGroundItemSystem::WorldGroundItemSystem() :
    _nextIdGroundItem( 1 ) {
}

Engine::GroundItemModel WorldGroundItemSystem::drop( const QString& idItem, const Engine::EntityPositionModel& position ) {
    Engine::GroundItemModel groundItem;
    groundItem.setIdGroundItem( _nextIdGroundItem++ );
    groundItem.setIdItem( idItem );
    groundItem.position().setX( position.x() );
    groundItem.position().setY( position.y() );
    groundItem.position().setZ( position.z() );

    _groundItems[ groundItem.idGroundItem() ] = groundItem;

    return groundItem;
}

bool WorldGroundItemSystem::remove( int idGroundItem ) {
    return _groundItems.erase( idGroundItem ) > 0;
}

std::optional<Engine::GroundItemModel> WorldGroundItemSystem::find( int idGroundItem ) const {
    const auto it = _groundItems.find( idGroundItem );
    if ( it == _groundItems.end() ) {
        return std::nullopt;
    }

    return it->second;
}

std::vector<Engine::GroundItemModel> WorldGroundItemSystem::itemsNear( const Engine::EntityPositionModel& position ) const {
    const int centerChunkX = position.x() / Engine::WorldConstants::CHUNK_SIZE;
    const int centerChunkY = position.y() / Engine::WorldConstants::CHUNK_SIZE;

    std::vector<Engine::GroundItemModel> result;

    for ( const auto& entry : _groundItems ) {
        const Engine::EntityPositionModel& itemPosition = entry.second.position();

        if ( itemPosition.z() != position.z() ) {
            continue;
        }

        const int chunkX = itemPosition.x() / Engine::WorldConstants::CHUNK_SIZE;
        const int chunkY = itemPosition.y() / Engine::WorldConstants::CHUNK_SIZE;

        if ( std::abs( chunkX - centerChunkX ) > 1 || std::abs( chunkY - centerChunkY ) > 1 ) {
            continue;
        }

        result.push_back( entry.second );
    }

    return result;
}

bool WorldGroundItemSystem::isReachable( const Engine::GroundItemModel& groundItem, const Engine::EntityPositionModel& position ) {
    const Engine::EntityPositionModel& itemPosition = groundItem.position();

    if ( itemPosition.z() != position.z() ) {
        return false;
    }

    return std::max( std::abs( itemPosition.x() - position.x() ), std::abs( itemPosition.y() - position.y() ) ) <= PICKUP_REACH_TILES;
}

} // namespace Server
