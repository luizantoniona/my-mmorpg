#ifndef CHARACTERVITALSSYSTEM_H
#define CHARACTERVITALSSYSTEM_H

#include <vector>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGServer/Server/Event/WorldEvent.h>
#include <MMORPGServer/Server/Runtime/Character/System/CharacterSystem.h>

namespace Server {

class CharacterVitalsSystem : public CharacterSystem {
public:
    CharacterVitalsSystem( Engine::CharacterModel* character, std::vector<WorldEvent>& pendingEvents, int tickRate );

    void onTick() override;

private:
    Engine::CharacterModel* _character;
    std::vector<WorldEvent>& _pendingEvents;
    int _ticksPerRegen;
    int _ticksSinceLastRegen;
};

} // namespace Server

#endif // CHARACTERVITALSSYSTEM_H
