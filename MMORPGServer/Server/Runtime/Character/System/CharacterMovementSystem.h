#ifndef CHARACTERMOVEMENTSYSTEM_H
#define CHARACTERMOVEMENTSYSTEM_H

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGServer/Server/Runtime/Character/System/CharacterSystem.h>

namespace Server {

class CharacterMovementSystem : public CharacterSystem {
public:
    explicit CharacterMovementSystem( Engine::CharacterModel* character );

    void onTick() override;

private:
    Engine::CharacterModel* _character;
};

} // namespace Server

#endif // CHARACTERMOVEMENTSYSTEM_H
