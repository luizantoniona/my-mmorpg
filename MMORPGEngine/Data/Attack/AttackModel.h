#ifndef ATTACKMODEL_H
#define ATTACKMODEL_H

#include <vector>

#include <MMORPGEngine/Data/Attack/AttackHitModeEnum.h>
#include <MMORPGEngine/Data/Attack/AttackStepModel.h>

namespace Engine {

class AttackModel {
public:
    AttackModel();

    static AttackModel basic();

    double castSeconds() const;
    void setCastSeconds( double castSeconds );

    AttackHitModeEnum hitMode() const;
    void setHitMode( AttackHitModeEnum hitMode );

    const std::vector<AttackStepModel>& steps() const;
    void setSteps( const std::vector<AttackStepModel>& steps );

private:
    std::vector<AttackStepModel> _steps;
    double _castSeconds;
    AttackHitModeEnum _hitMode;
};

} // namespace Engine

#endif // ATTACKMODEL_H
