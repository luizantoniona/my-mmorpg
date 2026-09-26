#ifndef CREATURERUNTIME_H
#define CREATURERUNTIME_H

#include <functional>
#include <memory>
#include <optional>

#include <MMORPGEngine/Entity/Creature/CreatureModel.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>

namespace Server {

class CreatureRuntime {
public:
    explicit CreatureRuntime( std::unique_ptr<Engine::CreatureModel> creature );

    Engine::CreatureModel* creature();
    const Engine::CreatureModel* creature() const;

    std::optional<Engine::EntityPositionModel> tick(
        const std::function<bool( const Engine::EntityPositionModel& )>& hasCharacterNear,
        const std::function<bool( const Engine::EntityPositionModel& )>& isPositionOccupied,
        int tickRate );

private:
    Engine::EntityPositionModel candidateStepPosition() const;
    void commitStep( const Engine::EntityPositionModel& position );

private:
    std::unique_ptr<Engine::CreatureModel> _creature;
    int _originX;
    int _originY;
    int _stepIndex;
};

} // namespace Server

#endif // CREATURERUNTIME_H
