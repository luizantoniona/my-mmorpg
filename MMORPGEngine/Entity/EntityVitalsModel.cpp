#include "EntityVitalsModel.h"

namespace Engine {

EntityVitalsModel::EntityVitalsModel() :
    _health( 0.0 ),
    _maxHealth( 0.0 ),
    _mana( 0.0 ),
    _maxMana( 0.0 ),
    _stamina( 0.0 ),
    _maxStamina( 0.0 ) {
}

EntityVitalsModel::~EntityVitalsModel() = default;

double EntityVitalsModel::health() const {
    return _health;
}

void EntityVitalsModel::setHealth( double health ) {
    _health = health;
}

double EntityVitalsModel::maxHealth() const {
    return _maxHealth;
}

void EntityVitalsModel::setMaxHealth( double maxHealth ) {
    _maxHealth = maxHealth;
}

double EntityVitalsModel::mana() const {
    return _mana;
}

void EntityVitalsModel::setMana( double mana ) {
    _mana = mana;
}

double EntityVitalsModel::maxMana() const {
    return _maxMana;
}

void EntityVitalsModel::setMaxMana( double maxMana ) {
    _maxMana = maxMana;
}

double EntityVitalsModel::stamina() const {
    return _stamina;
}

void EntityVitalsModel::setStamina( double stamina ) {
    _stamina = stamina;
}

double EntityVitalsModel::maxStamina() const {
    return _maxStamina;
}

void EntityVitalsModel::setMaxStamina( double maxStamina ) {
    _maxStamina = maxStamina;
}

} // namespace Engine
