#include "AttackStepModel.h"

namespace Engine {

AttackStepModel::AttackStepModel() :
    _delaySeconds( 0.0 ),
    _damageMultiplier( 1.0 ) {
}

double AttackStepModel::delaySeconds() const {
    return _delaySeconds;
}

void AttackStepModel::setDelaySeconds( double delaySeconds ) {
    _delaySeconds = delaySeconds;
}

double AttackStepModel::damageMultiplier() const {
    return _damageMultiplier;
}

void AttackStepModel::setDamageMultiplier( double damageMultiplier ) {
    _damageMultiplier = damageMultiplier;
}

const std::vector<AttackCellModel>& AttackStepModel::cells() const {
    return _cells;
}

void AttackStepModel::setCells( const std::vector<AttackCellModel>& cells ) {
    _cells = cells;
}

} // namespace Engine
