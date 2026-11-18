#include "WorldCombatAttackPlanner.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <utility>

namespace {

constexpr double TICK_EPSILON = 1e-9;
constexpr int DIRECTION_COUNT = 8;
constexpr double PI = 3.14159265358979323846;

int distanceSquared( const Engine::AttackCellModel& cell ) {
    return cell.forward() * cell.forward() + cell.lateral() * cell.lateral();
}

int directionIndex( int dx, int dy ) {
    if ( dx == 0 && dy == 0 ) {
        return 0;
    }

    const double angle = std::atan2( static_cast<double>( dx ), static_cast<double>( -dy ) );
    const int index = static_cast<int>( std::lround( angle / ( PI / 4.0 ) ) );

    return ( index % DIRECTION_COUNT + DIRECTION_COUNT ) % DIRECTION_COUNT;
}

int ringPosition( int x, int y, int radius ) {
    if ( y == -radius && x >= 0 ) {
        return x;
    }
    if ( x == radius ) {
        return 2 * radius + y;
    }
    if ( y == radius ) {
        return 4 * radius - x;
    }
    if ( x == -radius ) {
        return 6 * radius - y;
    }

    return 8 * radius + x;
}

std::pair<int, int> ringOffset( int position, int radius ) {
    if ( position <= radius ) {
        return std::make_pair( position, -radius );
    }
    if ( position <= 3 * radius ) {
        return std::make_pair( radius, position - 2 * radius );
    }
    if ( position <= 5 * radius ) {
        return std::make_pair( 4 * radius - position, radius );
    }
    if ( position <= 7 * radius ) {
        return std::make_pair( -radius, 6 * radius - position );
    }

    return std::make_pair( position - 8 * radius, -radius );
}

std::pair<int, int> rotatedOffset( const Engine::AttackCellModel& cell, int direction ) {
    const int x = cell.lateral();
    const int y = -cell.forward();
    const int radius = std::max( std::abs( x ), std::abs( y ) );

    if ( radius == 0 || direction == 0 ) {
        return std::make_pair( x, y );
    }

    const int perimeter = 8 * radius;
    const int position = ( ringPosition( x, y, radius ) + direction * radius ) % perimeter;

    return ringOffset( position, radius );
}

} // namespace

namespace Server {

std::vector<WorldCombatStrikeModel> WorldCombatAttackPlanner::plan( const Engine::AttackModel& attack, int originX, int originY, int dx, int dy, int tickRate ) {
    const int direction = directionIndex( dx, dy );

    std::vector<WorldCombatStrikeModel> strikes;

    for ( const Engine::AttackStepModel& step : attack.steps() ) {
        std::vector<Engine::AttackCellModel> ordered = step.cells();
        std::stable_sort( ordered.begin(), ordered.end(), []( const Engine::AttackCellModel& left, const Engine::AttackCellModel& right ) {
            return distanceSquared( left ) < distanceSquared( right );
        } );

        std::vector<std::pair<int, int>> cells;
        for ( const Engine::AttackCellModel& cell : ordered ) {
            const std::pair<int, int> offset = rotatedOffset( cell, direction );
            cells.emplace_back( originX + offset.first, originY + offset.second );
        }

        const double seconds = attack.castSeconds() + step.delaySeconds();
        const int ticks = std::max( 1, static_cast<int>( std::ceil( seconds * tickRate - TICK_EPSILON ) ) );

        strikes.push_back( WorldCombatStrikeModel( cells, attack.hitMode(), step.damageMultiplier(), seconds, ticks ) );
    }

    return strikes;
}

} // namespace Server
