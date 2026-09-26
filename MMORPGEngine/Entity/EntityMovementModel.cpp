#include "EntityMovementModel.h"

namespace {

// TODO: Fixed default for every entity today; revisit when level/stats can scale movement speed (Backlog "Movimento, footprint e orientação")
constexpr int DEFAULT_MOVEMENT_COOLDOWN = 20;

} // namespace

namespace Engine {

EntityMovementModel::EntityMovementModel() :
    _movementCooldown( DEFAULT_MOVEMENT_COOLDOWN ),
    _movementCounter( 0 ) {
}

int EntityMovementModel::cooldown() const {
    return _movementCooldown;
}

void EntityMovementModel::setCooldown( int cooldown ) {
    _movementCooldown = cooldown;
}

int EntityMovementModel::counter() const {
    return _movementCounter;
}

void EntityMovementModel::setCounter( int counter ) {
    _movementCounter = counter;
}

} // namespace Engine
