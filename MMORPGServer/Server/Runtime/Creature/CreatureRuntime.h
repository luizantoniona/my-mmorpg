#ifndef CREATURERUNTIME_H
#define CREATURERUNTIME_H

#include <functional>
#include <memory>
#include <optional>
#include <random>

#include <MMORPGEngine/Entity/Creature/CreatureModel.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGServer/Server/Runtime/Creature/CreaturePathfinder.h>
#include <MMORPGServer/Server/Runtime/World/Spawn/WorldSpawnAreaRuntime.h>

namespace Server {

class CreatureRuntime {
public:
    CreatureRuntime( std::unique_ptr<Engine::CreatureModel> creature, const WorldSpawnAreaRuntime* spawnArea );

    Engine::CreatureModel* creature();
    const Engine::CreatureModel* creature() const;

    bool isReturning() const;
    void updateLeash();

    std::optional<Engine::EntityPositionModel> tick(
        const std::function<bool( const Engine::EntityPositionModel& )>& hasCharacterNear,
        const CreaturePathfinder::PositionPredicate& isWalkable,
        const CreaturePathfinder::PositionPredicate& isOccupied,
        int tickRate );

    std::optional<Engine::EntityPositionModel> chase(
        const Engine::EntityPositionModel& targetPosition,
        int attackRange,
        const CreaturePathfinder::PositionPredicate& isWalkable,
        const CreaturePathfinder::PositionPredicate& isOccupied,
        int tickRate );

private:
    std::optional<Engine::EntityPositionModel> wanderStep( const CreaturePathfinder::PositionPredicate& isWalkable, const CreaturePathfinder::PositionPredicate& isOccupied );
    std::optional<Engine::EntityPositionModel> directStepToward( const Engine::EntityPositionModel& destination, const CreaturePathfinder::PositionPredicate& isWalkable, const CreaturePathfinder::PositionPredicate& isOccupied ) const;
    void commitStep( const Engine::EntityPositionModel& position );

    bool isInsideWanderArea( int x, int y ) const;
    Engine::EntityPositionModel nearestWanderAreaPosition() const;
    int distanceFromWanderArea() const;

private:
    std::unique_ptr<Engine::CreatureModel> _creature;
    const WorldSpawnAreaRuntime* _spawnArea;
    std::mt19937 _randomEngine;
    bool _returning;
};

} // namespace Server

#endif // CREATURERUNTIME_H
