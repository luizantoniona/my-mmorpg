#include "CreatureTypeVitalsModel.h"

namespace Engine {

CreatureTypeVitalsModel::CreatureTypeVitalsModel() :
    _maxHealth( 0.0 ) {
}

double CreatureTypeVitalsModel::maxHealth() const {
    return _maxHealth;
}

void CreatureTypeVitalsModel::setMaxHealth( double maxHealth ) {
    _maxHealth = maxHealth;
}

} // namespace Engine
