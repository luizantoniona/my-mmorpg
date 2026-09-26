#include "CharacterRuntime.h"

#include <MMORPGEngine/Entity/EntityMovementModel.h>

namespace Server {

CharacterRuntime::CharacterRuntime( std::unique_ptr<Engine::CharacterModel> character, EventBus& worldEventBus ) :
    _character( std::move( character ) ) {

    Engine::EntityMovementModel& movement = _character->movement();
    movement.setCounter( movement.cooldown() );
}

Engine::CharacterModel* CharacterRuntime::character() {
    return _character.get();
}

const Engine::CharacterModel* CharacterRuntime::character() const {
    return _character.get();
}

void CharacterRuntime::tick() {
    Engine::EntityMovementModel& movement = _character->movement();
    movement.setCounter( movement.counter() + 1 );

    for ( auto& system : _systems ) {
        system->onTick();
    }
}

} // namespace Server
