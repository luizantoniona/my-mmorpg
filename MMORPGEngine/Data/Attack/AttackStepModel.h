#ifndef ATTACKSTEPMODEL_H
#define ATTACKSTEPMODEL_H

#include <vector>

#include <MMORPGEngine/Data/Attack/AttackCellModel.h>

namespace Engine {

class AttackStepModel {
public:
    AttackStepModel();

    double delaySeconds() const;
    void setDelaySeconds( double delaySeconds );

    double damageMultiplier() const;
    void setDamageMultiplier( double damageMultiplier );

    const std::vector<AttackCellModel>& cells() const;
    void setCells( const std::vector<AttackCellModel>& cells );

private:
    std::vector<AttackCellModel> _cells;
    double _delaySeconds;
    double _damageMultiplier;
};

} // namespace Engine

#endif // ATTACKSTEPMODEL_H
