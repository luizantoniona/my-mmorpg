#ifndef WORLDCOMBATSTRIKEMODEL_H
#define WORLDCOMBATSTRIKEMODEL_H

#include <utility>
#include <vector>

#include <MMORPGEngine/Data/Attack/AttackHitModeEnum.h>

namespace Server {

class WorldCombatStrikeModel {
public:
    WorldCombatStrikeModel( std::vector<std::pair<int, int>> cells, Engine::AttackHitModeEnum hitMode, double damageMultiplier, double seconds, int ticks );

    const std::vector<std::pair<int, int>>& cells() const;

    Engine::AttackHitModeEnum hitMode() const;

    double damageMultiplier() const;

    double seconds() const;

    int ticks() const;

private:
    std::vector<std::pair<int, int>> _cells;
    double _damageMultiplier;
    double _seconds;
    Engine::AttackHitModeEnum _hitMode;
    int _ticks;
};

} // namespace Server

#endif // WORLDCOMBATSTRIKEMODEL_H
