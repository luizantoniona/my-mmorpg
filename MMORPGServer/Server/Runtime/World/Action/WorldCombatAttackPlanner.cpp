#include "WorldCombatAttackPlanner.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace {

constexpr double TICK_EPSILON = 1e-9;

int sign( int value ) {
    return ( value > 0 ) - ( value < 0 );
}

bool isLineShape( const Engine::AttackModel& attack ) {
    for ( const Engine::AttackStepModel& step : attack.steps() ) {
        for ( const Engine::AttackCellModel& cell : step.cells() ) {
            if ( cell.lateral() != 0 ) {
                return false;
            }
        }
    }

    return true;
}

int distanceSquared( const Engine::AttackCellModel& cell ) {
    return cell.forward() * cell.forward() + cell.lateral() * cell.lateral();
}

} // namespace

namespace Server {

std::vector<WorldCombatStrikeModel> WorldCombatAttackPlanner::plan( const Engine::AttackModel& attack, int originX, int originY, int dx, int dy, int tickRate ) {
    int facingX = sign( dx );
    int facingY = sign( dy );

    if ( facingX == 0 && facingY == 0 ) {
        facingY = -1;
    }

    const bool diagonal = facingX != 0 && facingY != 0;

    if ( diagonal && !isLineShape( attack ) ) {
        if ( std::abs( dy ) > std::abs( dx ) ) {
            facingX = 0;
        } else {
            facingY = 0;
        }
    }

    const int rightX = -facingY;
    const int rightY = facingX;

    std::vector<WorldCombatStrikeModel> strikes;

    for ( const Engine::AttackStepModel& step : attack.steps() ) {
        std::vector<Engine::AttackCellModel> ordered = step.cells();
        std::stable_sort( ordered.begin(), ordered.end(), []( const Engine::AttackCellModel& left, const Engine::AttackCellModel& right ) {
            return distanceSquared( left ) < distanceSquared( right );
        } );

        std::vector<std::pair<int, int>> cells;
        for ( const Engine::AttackCellModel& cell : ordered ) {
            cells.emplace_back( originX + cell.forward() * facingX + cell.lateral() * rightX, originY + cell.forward() * facingY + cell.lateral() * rightY );
        }

        const double seconds = attack.castSeconds() + step.delaySeconds();
        const int ticks = std::max( 1, static_cast<int>( std::ceil( seconds * tickRate - TICK_EPSILON ) ) );

        strikes.push_back( WorldCombatStrikeModel( cells, attack.hitMode(), step.damageMultiplier(), seconds, ticks ) );
    }

    return strikes;
}

} // namespace Server
