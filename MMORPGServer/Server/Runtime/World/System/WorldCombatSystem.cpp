#include "WorldCombatSystem.h"

#include <algorithm>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace Server {

WorldCombatSystem::WorldCombatSystem( WorldRuntime& runtime ) :
    WorldSystem( runtime ),
    _attackAction( nullptr ) {
    auto attackAction = std::make_unique<WorldCombatAttackAction>( runtime );
    _attackAction = attackAction.get();
    _actions.push_back( std::move( attackAction ) );
}

void WorldCombatSystem::onTick() {
    for ( auto& action : _actions ) {
        action->onTick();
    }
}

void WorldCombatSystem::handle( const CharacterAttackEntityCommand& command ) {
    attack( command.idCharacter(), command.dx(), command.dy() );
}

bool WorldCombatSystem::attack( int idCharacter, int dx, int dy ) {
    Engine::CharacterModel* character = _runtime.character( idCharacter );
    if ( !character ) {
        return false;
    }

    const Engine::EntityPositionModel currentPosition = character->position();

    return perform( Engine::CombatActionEnum::ATTACK, WorldCombatAttackerEnum::CHARACTER, idCharacter, currentPosition.x() + dx, currentPosition.y() + dy, currentPosition.z() );
}

bool WorldCombatSystem::attackCreature( int idCreature, int x, int y, int z ) {
    return perform( Engine::CombatActionEnum::ATTACK, WorldCombatAttackerEnum::CREATURE, idCreature, x, y, z );
}

void WorldCombatSystem::cancelCharacterActions( int idCharacter ) {
    for ( auto& action : _actions ) {
        action->cancel( WorldCombatAttackerEnum::CHARACTER, idCharacter );
    }
}

bool WorldCombatSystem::hasPendingCreatureAttack( int idCreature ) const {
    return _attackAction->isActive( WorldCombatAttackerEnum::CREATURE, idCreature );
}

bool WorldCombatSystem::resolveCharacterAttack( int idCharacter, int x, int y, int z, double damage ) {
    return _attackAction->resolveCharacterAttack( idCharacter, x, y, z, damage );
}

void WorldCombatSystem::resolveCreatureAttack( int idCreature, int x, int y, int z, double damage ) {
    _attackAction->resolveCreatureAttack( idCreature, x, y, z, damage );
}

bool WorldCombatSystem::perform( Engine::CombatActionEnum action, WorldCombatAttackerEnum attacker, int idAttacker, int targetX, int targetY, int targetZ ) {
    auto it = std::find_if( _actions.begin(), _actions.end(), [ action ]( const std::unique_ptr<WorldCombatAction>& candidate ) {
        return candidate->id() == action;
    } );

    if ( it == _actions.end() ) {
        return false;
    }

    return ( *it )->start( attacker, idAttacker, targetX, targetY, targetZ );
}

} // namespace Server
