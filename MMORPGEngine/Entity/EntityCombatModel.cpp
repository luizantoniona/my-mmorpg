#include "EntityCombatModel.h"

namespace {

constexpr int DEFAULT_ATTACK_COOLDOWN = 20;

} // namespace

namespace Engine {

EntityCombatModel::EntityCombatModel() :
    _attackCooldown( DEFAULT_ATTACK_COOLDOWN ),
    _attackCounter( 0 ) {
}

int EntityCombatModel::cooldown() const {
    return _attackCooldown;
}

void EntityCombatModel::setCooldown( int cooldown ) {
    _attackCooldown = cooldown;
}

int EntityCombatModel::counter() const {
    return _attackCounter;
}

void EntityCombatModel::setCounter( int counter ) {
    _attackCounter = counter;
}

bool EntityCombatModel::isReady() const {
    return _attackCounter >= _attackCooldown;
}

} // namespace Engine
