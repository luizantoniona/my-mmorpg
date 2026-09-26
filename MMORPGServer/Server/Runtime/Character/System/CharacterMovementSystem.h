#ifndef CHARACTERMOVEMENTSYSTEM_H
#define CHARACTERMOVEMENTSYSTEM_H

#include <MMORPGServer/Server/Runtime/Character/System/CharacterSystem.h>

namespace Engine {
class CharacterModel;
} // namespace Engine

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
