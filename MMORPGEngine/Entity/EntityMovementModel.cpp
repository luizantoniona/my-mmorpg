#include "EntityMovementModel.h"

#include <cmath>

namespace {

// TODO: Fixed default for every entity today; revisit when level/stats can scale movement speed (Backlog "Movimento, footprint e orientação")
constexpr double DEFAULT_MOVEMENT_COOLDOWN_SECONDS = 1.0;

} // namespace

namespace Engine {

EntityMovementModel::EntityMovementModel() :
    _movementCooldownSeconds( DEFAULT_MOVEMENT_COOLDOWN_SECONDS ),
    _movementCounter( 0 ) {
}

double EntityMovementModel::cooldownSeconds() const {
    return _movementCooldownSeconds;
}

void EntityMovementModel::setCooldownSeconds( double cooldownSeconds ) {
    _movementCooldownSeconds = cooldownSeconds;
}

int EntityMovementModel::counter() const {
    return _movementCounter;
}

void EntityMovementModel::setCounter( int counter ) {
    _movementCounter = counter;
}

int EntityMovementModel::cooldownTicks( int tickRate ) const {
    return static_cast<int>( std::lround( _movementCooldownSeconds * tickRate ) );
}

bool EntityMovementModel::isReady( int tickRate ) const {
    return _movementCounter >= cooldownTicks( tickRate );
}

} // namespace Engine
