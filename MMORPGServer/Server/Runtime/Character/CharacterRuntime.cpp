#include "CharacterRuntime.h"

#include <MMORPGEngine/Entity/EntityCombatModel.h>
#include <MMORPGEngine/Entity/EntityMovementModel.h>
#include <MMORPGServer/Server/Runtime/Character/System/CharacterCombatSystem.h>
#include <MMORPGServer/Server/Runtime/Character/System/CharacterMovementSystem.h>

namespace Server {

CharacterRuntime::CharacterRuntime( std::unique_ptr<Engine::CharacterModel> character, EventBus& worldEventBus, int tickRate ) :
    _character( std::move( character ) ) {

    Engine::EntityMovementModel& movement = _character->movement();
    movement.setCounter( movement.cooldownTicks( tickRate ) );

    Engine::EntityCombatModel& combat = _character->combat();
    combat.setCounter( combat.cooldownTicks( tickRate ) );

    _systems.push_back( std::make_unique<CharacterMovementSystem>( _character.get() ) );
    _systems.push_back( std::make_unique<CharacterCombatSystem>( _character.get() ) );
}

Engine::CharacterModel* CharacterRuntime::character() {
    return _character.get();
}

const Engine::CharacterModel* CharacterRuntime::character() const {
    return _character.get();
}

void CharacterRuntime::tick() {
    for ( auto& system : _systems ) {
        system->onTick();
    }
}

} // namespace Server
