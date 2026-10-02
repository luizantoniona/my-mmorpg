#ifndef CREATUREPATHFINDER_H
#define CREATUREPATHFINDER_H

#include <functional>
#include <optional>

#include <MMORPGEngine/Entity/EntityPositionModel.h>

namespace Server {

class CreaturePathfinder {
public:
    using PositionPredicate = std::function<bool( int x, int y, int z )>;

    static std::optional<Engine::EntityPositionModel> nextStep(
        const Engine::EntityPositionModel& from,
        const Engine::EntityPositionModel& goal,
        int reach,
        const PositionPredicate& isWalkable,
        const PositionPredicate& isOccupied );
};

} // namespace Server

#endif // CREATUREPATHFINDER_H
