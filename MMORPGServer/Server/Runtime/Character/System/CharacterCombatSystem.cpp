#include "CharacterCombatSystem.h"

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/EntityCombatModel.h>

namespace Server {

CharacterCombatSystem::CharacterCombatSystem( Engine::CharacterModel* character ) :
    _character( character ) {
}

void CharacterCombatSystem::onTick() {
    Engine::EntityCombatModel& combat = _character->combat();
    combat.setCounter( combat.counter() + 1 );
}

} // namespace Server
