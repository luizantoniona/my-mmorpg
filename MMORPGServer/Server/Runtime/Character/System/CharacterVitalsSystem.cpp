#include "CharacterVitalsSystem.h"

#include <algorithm>

#include <MMORPGEngine/Entity/EntityVitalsModel.h>

namespace Server {

// TODO: reavaliar a regeneração (valor fixo por segundo) quando houver consumíveis e se mana/vida/estamina ganharem proficiência própria
static constexpr double REGEN_PER_SECOND = 1.0;

CharacterVitalsSystem::CharacterVitalsSystem( Engine::CharacterModel* character, std::vector<WorldEvent>& pendingEvents, int tickRate ) :
    _character( character ),
    _pendingEvents( pendingEvents ),
    _ticksPerRegen( std::max( 1, tickRate ) ),
    _ticksSinceLastRegen( 0 ) {
}

void CharacterVitalsSystem::onTick() {
    if ( ++_ticksSinceLastRegen < _ticksPerRegen ) {
        return;
    }

    _ticksSinceLastRegen = 0;

    Engine::EntityVitalsModel& vitals = _character->vitals();
    const double healthBefore = vitals.health();
    const double manaBefore = vitals.mana();
    const double staminaBefore = vitals.stamina();

    vitals.setHealth( std::min( vitals.maxHealth(), vitals.health() + REGEN_PER_SECOND ) );
    vitals.setMana( std::min( vitals.maxMana(), vitals.mana() + REGEN_PER_SECOND ) );
    vitals.setStamina( std::min( vitals.maxStamina(), vitals.stamina() + REGEN_PER_SECOND ) );

    if ( vitals.health() == healthBefore && vitals.mana() == manaBefore && vitals.stamina() == staminaBefore ) {
        return;
    }

    Json::Value payload;
    payload[ "idCharacter" ] = _character->idCharacter();
    _pendingEvents.push_back( WorldEvent( WorldEventType::CHARACTER_VITALS_CHANGED, payload ) );
}

} // namespace Server
