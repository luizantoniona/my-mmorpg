#ifndef CHARACTERRUNTIME_H
#define CHARACTERRUNTIME_H

#include <memory>
#include <vector>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGServer/Server/Event/EventBus.h>
#include <MMORPGServer/Server/Runtime/Character/System/CharacterSystem.h>

namespace Server {

class CharacterRuntime {
public:
    CharacterRuntime( std::unique_ptr<Engine::CharacterModel> character, EventBus& worldEventBus, int tickRate );

    Engine::CharacterModel* character();
    const Engine::CharacterModel* character() const;

    void tick();

private:
    std::unique_ptr<Engine::CharacterModel> _character;
    std::vector<std::unique_ptr<CharacterSystem>> _systems;
};

} // namespace Server

#endif // CHARACTERRUNTIME_H
