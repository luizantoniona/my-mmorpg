#include "CharacterRuntime.h"

#include <utility>

#include <MMORPGEngine/Entity/EntityCombatModel.h>
#include <MMORPGEngine/Entity/EntityMovementModel.h>
#include <MMORPGServer/Server/Runtime/Character/CharacterCombatActionResolver.h>
#include <MMORPGServer/Server/Runtime/Character/System/CharacterCombatSystem.h>
#include <MMORPGServer/Server/Runtime/Character/System/CharacterMovementSystem.h>
#include <MMORPGServer/Server/Runtime/Character/System/CharacterVitalsSystem.h>

namespace Server {

CharacterRuntime::CharacterRuntime( std::unique_ptr<Engine::CharacterModel> character, EventBus& worldEventBus, int tickRate ) :
    _character( std::move( character ) ),
    _equipmentSystem( nullptr ) {

    Engine::EntityMovementModel& movement = _character->movement();
    movement.setCounter( movement.cooldownTicks( tickRate ) );

    Engine::EntityCombatModel& combat = _character->combat();
    combat.setCounter( combat.cooldownTicks( tickRate ) );
    combat.setAvailableActions( CharacterCombatActionResolver::resolve( _character->equipment() ) );

    auto equipmentSystem = std::make_unique<CharacterEquipmentSystem>( _character.get(), _pendingEvents );
    _equipmentSystem = equipmentSystem.get();

    _systems.push_back( std::make_unique<CharacterMovementSystem>( _character.get() ) );
    _systems.push_back( std::make_unique<CharacterCombatSystem>( _character.get() ) );
    _systems.push_back( std::make_unique<CharacterVitalsSystem>( _character.get(), _pendingEvents, tickRate ) );
    _systems.push_back( std::move( equipmentSystem ) );
}

CharacterEquipmentSystem& CharacterRuntime::equipmentSystem() {
    return *_equipmentSystem;
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

std::vector<WorldEvent> CharacterRuntime::takePendingEvents() {
    return std::exchange( _pendingEvents, {} );
}

} // namespace Server
