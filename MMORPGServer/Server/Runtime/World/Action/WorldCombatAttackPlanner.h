#ifndef WORLDCOMBATATTACKPLANNER_H
#define WORLDCOMBATATTACKPLANNER_H

#include <vector>

#include <MMORPGEngine/Data/Attack/AttackModel.h>
#include <MMORPGServer/Server/Runtime/World/Action/WorldCombatStrikeModel.h>

namespace Server {

class WorldCombatAttackPlanner {
public:
    static std::vector<WorldCombatStrikeModel> plan( const Engine::AttackModel& attack, int originX, int originY, int dx, int dy, int tickRate );
};

} // namespace Server

#endif // WORLDCOMBATATTACKPLANNER_H
