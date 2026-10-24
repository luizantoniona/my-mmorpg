#include "AttackModel.h"

namespace Engine {

AttackModel::AttackModel() :
    _castSeconds( 0.5 ),
    _hitMode( AttackHitModeEnum::FIRST ) {
}

AttackModel AttackModel::basic() {
    AttackStepModel step;
    step.setCells( { AttackCellModel( 1, 0 ) } );

    AttackModel attack;
    attack.setSteps( { step } );

    return attack;
}

double AttackModel::castSeconds() const {
    return _castSeconds;
}

void AttackModel::setCastSeconds( double castSeconds ) {
    _castSeconds = castSeconds;
}

AttackHitModeEnum AttackModel::hitMode() const {
    return _hitMode;
}

void AttackModel::setHitMode( AttackHitModeEnum hitMode ) {
    _hitMode = hitMode;
}

const std::vector<AttackStepModel>& AttackModel::steps() const {
    return _steps;
}

void AttackModel::setSteps( const std::vector<AttackStepModel>& steps ) {
    _steps = steps;
}

} // namespace Engine
