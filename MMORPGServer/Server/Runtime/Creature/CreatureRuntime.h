#ifndef CREATURERUNTIME_H
#define CREATURERUNTIME_H

#include <memory>

#include <MMORPGEngine/Entity/Creature/CreatureModel.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>

namespace Server {

class CreatureRuntime {
public:
    explicit CreatureRuntime( std::unique_ptr<Engine::CreatureModel> creature );

    Engine::CreatureModel* creature();
    const Engine::CreatureModel* creature() const;

    Engine::EntityPositionModel candidateStepPosition() const;
    void commitStep();

private:
    std::unique_ptr<Engine::CreatureModel> _creature;
    int _originX;
    int _originY;
    int _stepIndex;
};

} // namespace Server

#endif // CREATURERUNTIME_H
