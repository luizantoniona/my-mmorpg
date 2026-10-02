#include "CharacterMovementSystem.h"

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/EntityMovementModel.h>

namespace Server {

CharacterMovementSystem::CharacterMovementSystem( Engine::CharacterModel* character ) :
    _character( character ) {
}

void CharacterMovementSystem::onTick() {
    Engine::EntityMovementModel& movement = _character->movement();
    movement.setCounter( movement.counter() + 1 );
}

} // namespace Server
