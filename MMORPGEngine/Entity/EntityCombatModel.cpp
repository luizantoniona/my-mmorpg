#include "EntityCombatModel.h"

#include <cmath>

namespace {

constexpr double DEFAULT_ATTACK_COOLDOWN_SECONDS = 1.0;

} // namespace

namespace Engine {

EntityCombatModel::EntityCombatModel() :
    _attackCooldownSeconds( DEFAULT_ATTACK_COOLDOWN_SECONDS ),
    _attackCounter( 0 ) {
}

double EntityCombatModel::cooldownSeconds() const {
    return _attackCooldownSeconds;
}

void EntityCombatModel::setCooldownSeconds( double cooldownSeconds ) {
    _attackCooldownSeconds = cooldownSeconds;
}

int EntityCombatModel::counter() const {
    return _attackCounter;
}

void EntityCombatModel::setCounter( int counter ) {
    _attackCounter = counter;
}

int EntityCombatModel::cooldownTicks( int tickRate ) const {
    return static_cast<int>( std::lround( _attackCooldownSeconds * tickRate ) );
}

bool EntityCombatModel::isReady( int tickRate ) const {
    return _attackCounter >= cooldownTicks( tickRate );
}

} // namespace Engine
