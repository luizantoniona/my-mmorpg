#ifndef CHARACTERCOMBATSYSTEM_H
#define CHARACTERCOMBATSYSTEM_H

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGServer/Server/Runtime/Character/System/CharacterSystem.h>

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
