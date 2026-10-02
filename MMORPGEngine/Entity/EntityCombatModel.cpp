#include "EntityCombatModel.h"

#include <cmath>

namespace {

constexpr double DEFAULT_ATTACK_COOLDOWN_SECONDS = 1.0;
constexpr int DEFAULT_ATTACK_RANGE = 1;

} // namespace

namespace Engine {

EntityCombatModel::EntityCombatModel() :
    _availableActions( { CombatActionEnum::ATTACK, CombatActionEnum::DODGE } ),
    _attackCooldownSeconds( DEFAULT_ATTACK_COOLDOWN_SECONDS ),
    _attackCounter( 0 ),
    _attackRange( DEFAULT_ATTACK_RANGE ) {
}

bool EntityCombatModel::isActionAvailable( CombatActionEnum action ) const {
    return _availableActions.count( action ) > 0;
}

void EntityCombatModel::setActionAvailable( CombatActionEnum action, bool available ) {
    if ( available ) {
        _availableActions.insert( action );
    } else {
        _availableActions.erase( action );
    }
}

const std::set<CombatActionEnum>& EntityCombatModel::availableActions() const {
    return _availableActions;
}

void EntityCombatModel::setAvailableActions( const std::set<CombatActionEnum>& availableActions ) {
    _availableActions = availableActions;
}

double EntityCombatModel::cooldownSeconds() const {
    return _attackCooldownSeconds;
}

void EntityCombatModel::setCooldownSeconds( double cooldownSeconds ) {
    _attackCooldownSeconds = cooldownSeconds;
}

int EntityCombatModel::attackRange() const {
    return _attackRange;
}

void EntityCombatModel::setAttackRange( int attackRange ) {
    _attackRange = attackRange;
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
