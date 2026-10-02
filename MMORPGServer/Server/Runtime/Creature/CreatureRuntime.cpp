#include "CreatureRuntime.h"

#include <algorithm>
#include <cstdlib>
#include <vector>

#include <MMORPGEngine/Entity/EntityMovementModel.h>

namespace {

constexpr int LEASH_DISTANCE = 12;

} // namespace

namespace Server {

CreatureRuntime::CreatureRuntime( std::unique_ptr<Engine::CreatureModel> creature, const WorldSpawnAreaRuntime* spawnArea ) :
    _creature( std::move( creature ) ),
    _spawnArea( spawnArea ),
    _randomEngine( std::random_device{}() ),
    _returning( false ) {
}

Engine::CreatureModel* CreatureRuntime::creature() {
    return _creature.get();
}

const Engine::CreatureModel* CreatureRuntime::creature() const {
    return _creature.get();
}

bool CreatureRuntime::isReturning() const {
    return _returning;
}

void CreatureRuntime::updateLeash() {
    const int distance = distanceFromWanderArea();

    if ( distance > LEASH_DISTANCE ) {
        _returning = true;
    } else if ( _returning && distance == 0 ) {
        _returning = false;
    }
}

std::optional<Engine::EntityPositionModel> CreatureRuntime::tick(
    const std::function<bool( const Engine::EntityPositionModel& )>& hasCharacterNear,
    const CreaturePathfinder::PositionPredicate& isWalkable,
    const CreaturePathfinder::PositionPredicate& isOccupied,
    int tickRate ) {

    if ( !hasCharacterNear( _creature->position() ) ) {
        return std::nullopt;
    }

    Engine::EntityMovementModel& movement = _creature->movement();
    movement.setCounter( movement.counter() + 1 );

    if ( !movement.isReady( tickRate ) ) {
        return std::nullopt;
    }

    const auto step = distanceFromWanderArea() > 0 ? directStepToward( nearestWanderAreaPosition(), isWalkable, isOccupied ) : wanderStep( isWalkable, isOccupied );
    if ( !step ) {
        return std::nullopt;
    }

    commitStep( *step );

    return step;
}

std::optional<Engine::EntityPositionModel> CreatureRuntime::chase(
    const Engine::EntityPositionModel& targetPosition,
    int attackRange,
    const CreaturePathfinder::PositionPredicate& isWalkable,
    const CreaturePathfinder::PositionPredicate& isOccupied,
    int tickRate ) {

    Engine::EntityMovementModel& movement = _creature->movement();
    movement.setCounter( movement.counter() + 1 );

    if ( !movement.isReady( tickRate ) ) {
        return std::nullopt;
    }

    const auto step = CreaturePathfinder::nextStep( _creature->position(), targetPosition, attackRange, isWalkable, isOccupied );
    if ( !step ) {
        return std::nullopt;
    }

    commitStep( *step );

    return step;
}

std::optional<Engine::EntityPositionModel> CreatureRuntime::wanderStep( const CreaturePathfinder::PositionPredicate& isWalkable, const CreaturePathfinder::PositionPredicate& isOccupied ) {
    const Engine::EntityPositionModel& current = _creature->position();

    std::vector<Engine::EntityPositionModel> candidates;

    for ( int dx = -1; dx <= 1; ++dx ) {
        for ( int dy = -1; dy <= 1; ++dy ) {
            const int x = current.x() + dx;
            const int y = current.y() + dy;

            if ( ( dx == 0 && dy == 0 ) || !isInsideWanderArea( x, y ) || !isWalkable( x, y, current.z() ) || isOccupied( x, y, current.z() ) ) {
                continue;
            }

            Engine::EntityPositionModel candidate = current;
            candidate.setX( x );
            candidate.setY( y );
            candidates.push_back( candidate );
        }
    }

    if ( candidates.empty() ) {
        return std::nullopt;
    }

    std::uniform_int_distribution<size_t> distribution( 0, candidates.size() - 1 );

    return candidates[ distribution( _randomEngine ) ];
}

std::optional<Engine::EntityPositionModel> CreatureRuntime::directStepToward(
    const Engine::EntityPositionModel& destination,
    const CreaturePathfinder::PositionPredicate& isWalkable,
    const CreaturePathfinder::PositionPredicate& isOccupied ) const {

    const Engine::EntityPositionModel& current = _creature->position();

    const int stepX = current.x() + ( destination.x() > current.x() ) - ( destination.x() < current.x() );
    const int stepY = current.y() + ( destination.y() > current.y() ) - ( destination.y() < current.y() );

    if ( !isWalkable( stepX, stepY, current.z() ) || isOccupied( stepX, stepY, current.z() ) ) {
        return std::nullopt;
    }

    Engine::EntityPositionModel position = current;
    position.setX( stepX );
    position.setY( stepY );

    return position;
}

void CreatureRuntime::commitStep( const Engine::EntityPositionModel& position ) {
    _creature->movement().setCounter( 0 );
    _creature->position() = position;
}

bool CreatureRuntime::isInsideWanderArea( int x, int y ) const {
    return _spawnArea && _spawnArea->isInside( x, y );
}

Engine::EntityPositionModel CreatureRuntime::nearestWanderAreaPosition() const {
    Engine::EntityPositionModel position = _creature->position();

    if ( !_spawnArea ) {
        return position;
    }

    const Engine::CreatureSpawnAreaModel& area = _spawnArea->area();
    position.setX( std::clamp( position.x(), area.x(), area.x() + static_cast<int>( area.width() ) - 1 ) );
    position.setY( std::clamp( position.y(), area.y(), area.y() + static_cast<int>( area.height() ) - 1 ) );

    return position;
}

int CreatureRuntime::distanceFromWanderArea() const {
    const Engine::EntityPositionModel& position = _creature->position();
    const Engine::EntityPositionModel nearest = nearestWanderAreaPosition();

    return std::max( std::abs( position.x() - nearest.x() ), std::abs( position.y() - nearest.y() ) );
}

} // namespace Server
