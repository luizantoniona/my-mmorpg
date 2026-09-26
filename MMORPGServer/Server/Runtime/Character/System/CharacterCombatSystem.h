#ifndef CHARACTERCOMBATSYSTEM_H
#define CHARACTERCOMBATSYSTEM_H

#include <MMORPGServer/Server/Runtime/Character/System/CharacterSystem.h>

namespace Engine {
class CharacterModel;
} // namespace Engine

namespace Server {

class CharacterCombatSystem : public CharacterSystem {
public:
    explicit CharacterCombatSystem( Engine::CharacterModel* character );

    void onTick() override;

private:
    Engine::CharacterModel* _character;
};

} // namespace Server

#endif // CHARACTERCOMBATSYSTEM_H
