#include "CreatureRuntime.h"

namespace Server {

CreatureRuntime::CreatureRuntime( std::unique_ptr<Engine::CreatureModel> creature ) :
    _creature( std::move( creature ) ) {
}

Engine::CreatureModel* CreatureRuntime::creature() {
    return _creature.get();
}

const Engine::CreatureModel* CreatureRuntime::creature() const {
    return _creature.get();
}

} // namespace Server
