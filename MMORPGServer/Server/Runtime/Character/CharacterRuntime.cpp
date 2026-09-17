#include "CharacterRuntime.h"

namespace Server {

CharacterRuntime::CharacterRuntime( std::unique_ptr<Engine::CharacterModel> character, EventBus& worldEventBus ) :
    _character( std::move( character ) ) {
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
