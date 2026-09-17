#ifndef CREATURERUNTIME_H
#define CREATURERUNTIME_H

#include <memory>

#include <MMORPGEngine/Entity/Creature/CreatureModel.h>

namespace Server {

class CreatureRuntime {
public:
    explicit CreatureRuntime( std::unique_ptr<Engine::CreatureModel> creature );

    Engine::CreatureModel* creature();
    const Engine::CreatureModel* creature() const;

private:
    std::unique_ptr<Engine::CreatureModel> _creature;
};

} // namespace Server

#endif // CREATURERUNTIME_H
