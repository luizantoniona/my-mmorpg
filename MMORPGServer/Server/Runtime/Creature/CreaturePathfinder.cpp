#include "CreaturePathfinder.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {

constexpr int MAX_EXPANDED_NODES = 400;

int64_t packKey( int x, int y ) {
    return ( static_cast<int64_t>( x ) << 32 ) | static_cast<uint32_t>( y );
}

int unpackX( int64_t key ) {
    return static_cast<int>( key >> 32 );
}

int unpackY( int64_t key ) {
    return static_cast<int>( static_cast<uint32_t>( key & 0xFFFFFFFF ) );
}

int distance( int x, int y, int goalX, int goalY ) {
    return std::max( std::abs( x - goalX ), std::abs( y - goalY ) );
}

} // namespace

namespace Server {

std::optional<Engine::EntityPositionModel> CreaturePathfinder::nextStep(
    const Engine::EntityPositionModel& from,
    const Engine::EntityPositionModel& goal,
    int reach,
    const PositionPredicate& isWalkable,
    const PositionPredicate& isOccupied ) {

    if ( from.z() != goal.z() || distance( from.x(), from.y(), goal.x(), goal.y() ) <= reach ) {
        return std::nullopt;
    }

    const int z = from.z();
    const int64_t startKey = packKey( from.x(), from.y() );

    using QueueEntry = std::pair<int, int64_t>;
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, std::greater<QueueEntry>> open;
    std::unordered_map<int64_t, int64_t> parents;
    std::unordered_map<int64_t, int> costs;

    open.push( QueueEntry( distance( from.x(), from.y(), goal.x(), goal.y() ), startKey ) );
    parents[ startKey ] = startKey;
    costs[ startKey ] = 0;

    int expandedNodes = 0;

    while ( !open.empty() && expandedNodes < MAX_EXPANDED_NODES ) {
        const int64_t currentKey = open.top().second;
        open.pop();
        ++expandedNodes;

        const int currentX = unpackX( currentKey );
        const int currentY = unpackY( currentKey );

        if ( distance( currentX, currentY, goal.x(), goal.y() ) <= reach ) {
            int64_t stepKey = currentKey;
            while ( parents[ stepKey ] != startKey ) {
                stepKey = parents[ stepKey ];
            }

            Engine::EntityPositionModel step = from;
            step.setX( unpackX( stepKey ) );
            step.setY( unpackY( stepKey ) );
            return step;
        }

        const int currentCost = costs[ currentKey ];

        for ( int dx = -1; dx <= 1; ++dx ) {
            for ( int dy = -1; dy <= 1; ++dy ) {
                if ( dx == 0 && dy == 0 ) {
                    continue;
                }

                const int nextX = currentX + dx;
                const int nextY = currentY + dy;
                const int64_t nextKey = packKey( nextX, nextY );
                const int nextCost = currentCost + 1;

                const auto knownCost = costs.find( nextKey );
                if ( knownCost != costs.end() && knownCost->second <= nextCost ) {
                    continue;
                }

                if ( !isWalkable( nextX, nextY, z ) || isOccupied( nextX, nextY, z ) ) {
                    continue;
                }

                costs[ nextKey ] = nextCost;
                parents[ nextKey ] = currentKey;
                open.push( QueueEntry( nextCost + distance( nextX, nextY, goal.x(), goal.y() ), nextKey ) );
            }
        }
    }

    return std::nullopt;
}

} // namespace Server
