#include "WorldCombatStrikeModel.h"

namespace Server {

WorldCombatStrikeModel::WorldCombatStrikeModel( std::vector<std::pair<int, int>> cells, Engine::AttackHitModeEnum hitMode, double damageMultiplier, double seconds, int ticks ) :
    _cells( std::move( cells ) ),
    _damageMultiplier( damageMultiplier ),
    _seconds( seconds ),
    _hitMode( hitMode ),
    _ticks( ticks ) {
}

const std::vector<std::pair<int, int>>& WorldCombatStrikeModel::cells() const {
    return _cells;
}

Engine::AttackHitModeEnum WorldCombatStrikeModel::hitMode() const {
    return _hitMode;
}

double WorldCombatStrikeModel::damageMultiplier() const {
    return _damageMultiplier;
}

double WorldCombatStrikeModel::seconds() const {
    return _seconds;
}

int WorldCombatStrikeModel::ticks() const {
    return _ticks;
}

} // namespace Server
