#include "CreatureRuntime.h"

#include <MMORPGEngine/Entity/EntityMovementModel.h>

namespace {

// TODO: Placeholder movement to see creatures move in real time (Fase 7 passo 5 replaces this with A* IA)
constexpr int STEP_OFFSETS[ 4 ][ 2 ] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };

} // namespace

namespace Server {

CreatureRuntime::CreatureRuntime( std::unique_ptr<Engine::CreatureModel> creature ) :
    _creature( std::move( creature ) ),
    _originX( _creature->position().x() ),
    _originY( _creature->position().y() ),
    _stepIndex( 0 ) {
}

Engine::CreatureModel* CreatureRuntime::creature() {
    return _creature.get();
}

const Engine::CreatureModel* CreatureRuntime::creature() const {
    return _creature.get();
}

std::optional<Engine::EntityPositionModel> CreatureRuntime::tick(
    const std::function<bool( const Engine::EntityPositionModel& )>& hasCharacterNear,
    const std::function<bool( const Engine::EntityPositionModel& )>& isPositionOccupied,
    int tickRate ) {

    if ( !hasCharacterNear( _creature->position() ) ) {
        return std::nullopt;
    }

    Engine::EntityMovementModel& movement = _creature->movement();
    movement.setCounter( movement.counter() + 1 );

    if ( !movement.isReady( tickRate ) ) {
        return std::nullopt;
    }

    const Engine::EntityPositionModel candidate = candidateStepPosition();
    if ( isPositionOccupied( candidate ) ) {
        return std::nullopt;
    }

    commitStep( candidate );

    return candidate;
}

Engine::EntityPositionModel CreatureRuntime::candidateStepPosition() const {
    const int nextStepIndex = ( _stepIndex + 1 ) % 4;

    Engine::EntityPositionModel position = _creature->position();
    position.setX( _originX + STEP_OFFSETS[ nextStepIndex ][ 0 ] );
    position.setY( _originY + STEP_OFFSETS[ nextStepIndex ][ 1 ] );

    return position;
}

void CreatureRuntime::commitStep( const Engine::EntityPositionModel& position ) {
    _stepIndex = ( _stepIndex + 1 ) % 4;

    _creature->movement().setCounter( 0 );
    _creature->position() = position;
}

} // namespace Server
