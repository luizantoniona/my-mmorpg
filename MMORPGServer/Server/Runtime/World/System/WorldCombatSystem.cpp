#include "WorldCombatSystem.h"

#include <algorithm>
#include <cmath>

#include <QDebug>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/Creature/CreatureModel.h>
#include <MMORPGServer/Server/Event/WorldEvent.h>
#include <MMORPGServer/Server/Event/WorldEventType.h>
#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace {

constexpr int NO_CREATURE = -1;
constexpr double ATTACK_DAMAGE = 5.0;
constexpr double ATTACK_STAMINA_COST = 10.0;

// TODO: duração do cast e dano de criatura fixos por ora; depois vêm da arma equipada (ItemType) e do catálogo de criatura
constexpr double ATTACK_CAST_SECONDS = 0.5;
constexpr double CREATURE_ATTACK_DAMAGE = 2.0;

} // namespace

namespace Server {

WorldCombatSystem::WorldCombatSystem( WorldRuntime& runtime ) :
    WorldSystem( runtime ) {
}

void WorldCombatSystem::onTick() {
    std::vector<PendingAttack> dueAttacks;

    for ( PendingAttack& pendingAttack : _pendingAttacks ) {
        --pendingAttack.ticksRemaining;
    }

    auto firstDue = std::stable_partition( _pendingAttacks.begin(), _pendingAttacks.end(), []( const PendingAttack& pendingAttack ) {
        return pendingAttack.ticksRemaining > 0;
    } );

    dueAttacks.assign( firstDue, _pendingAttacks.end() );
    _pendingAttacks.erase( firstDue, _pendingAttacks.end() );

    for ( const PendingAttack& pendingAttack : dueAttacks ) {
        if ( pendingAttack.attacker == WorldCombatAttackerEnum::CHARACTER ) {
            resolveCharacterAttack( pendingAttack.idAttacker, pendingAttack.x, pendingAttack.y, pendingAttack.z, pendingAttack.damage );
        } else {
            resolveCreatureAttack( pendingAttack.idAttacker, pendingAttack.x, pendingAttack.y, pendingAttack.z, pendingAttack.damage );
        }
    }
}

bool WorldCombatSystem::attack( int idCharacter, int dx, int dy ) {
    Engine::CharacterModel* character = _runtime.character( idCharacter );
    if ( !character ) {
        return false;
    }

    const Engine::EntityPositionModel currentPosition = character->position();
    const int targetX = currentPosition.x() + dx;
    const int targetY = currentPosition.y() + dy;
    const int z = currentPosition.z();

    const bool hasStamina = character->vitals().stamina() >= ATTACK_STAMINA_COST;
    const bool attackDue = character->combat().isReady( _runtime.tickRate() );
    const bool casting = hasPendingAttack( WorldCombatAttackerEnum::CHARACTER, idCharacter );

    if ( !hasStamina || !attackDue || casting ) {
        qInfo() << "[WorldCombatSystem] Attack blocked [CHARACTER]" << idCharacter << "[HAS_STAMINA]" << hasStamina
                << "[ATTACK_DUE]" << attackDue << "[CASTING]" << casting;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock( _runtime.mutex() );

        Engine::CharacterModel* characterPtr = _runtime.characterLocked( idCharacter );
        if ( !characterPtr ) {
            return false;
        }

        characterPtr->vitals().setStamina( std::max( 0.0, characterPtr->vitals().stamina() - ATTACK_STAMINA_COST ) );
        characterPtr->combat().setCounter( 0 );
    }

    _pendingAttacks.push_back( PendingAttack( WorldCombatAttackerEnum::CHARACTER, idCharacter, targetX, targetY, z, ATTACK_DAMAGE, castTicks() ) );

    Json::Value startedPayload;
    startedPayload[ "idCharacter" ] = idCharacter;
    startedPayload[ "x" ] = targetX;
    startedPayload[ "y" ] = targetY;
    startedPayload[ "z" ] = z;
    startedPayload[ "castSeconds" ] = ATTACK_CAST_SECONDS;
    _runtime.eventBus().publish( WorldEvent( WorldEventType::CHARACTER_ATTACK_STARTED, startedPayload ) );

    Json::Value vitalsPayload;
    vitalsPayload[ "idCharacter" ] = idCharacter;
    _runtime.eventBus().publish( WorldEvent( WorldEventType::CHARACTER_VITALS_CHANGED, vitalsPayload ) );

    qInfo() << "[WorldCombatSystem] Character started attack [CHARACTER]" << idCharacter << "[X]" << targetX << "[Y]" << targetY << "[Z]" << z;

    return true;
}

bool WorldCombatSystem::attackCreature( int idCreature, int x, int y, int z ) {
    {
        std::lock_guard<std::mutex> lock( _runtime.mutex() );

        auto& creatures = _runtime.creaturesLocked();
        auto it = creatures.find( idCreature );
        if ( it == creatures.end() ) {
            return false;
        }

        it->second->creature()->combat().setCounter( 0 );
    }

    _pendingAttacks.push_back( PendingAttack( WorldCombatAttackerEnum::CREATURE, idCreature, x, y, z, CREATURE_ATTACK_DAMAGE, castTicks() ) );

    Json::Value startedPayload;
    startedPayload[ "idCreature" ] = idCreature;
    startedPayload[ "x" ] = x;
    startedPayload[ "y" ] = y;
    startedPayload[ "z" ] = z;
    startedPayload[ "castSeconds" ] = ATTACK_CAST_SECONDS;
    _runtime.eventBus().publish( WorldEvent( WorldEventType::CREATURE_ATTACK_STARTED, startedPayload ) );

    return true;
}

void WorldCombatSystem::cancelCharacterAttack( int idCharacter ) {
    _pendingAttacks.erase( std::remove_if( _pendingAttacks.begin(), _pendingAttacks.end(), [ idCharacter ]( const PendingAttack& pendingAttack ) {
                               return pendingAttack.attacker == WorldCombatAttackerEnum::CHARACTER && pendingAttack.idAttacker == idCharacter;
                           } ),
                           _pendingAttacks.end() );
}

bool WorldCombatSystem::hasPendingCreatureAttack( int idCreature ) const {
    return hasPendingAttack( WorldCombatAttackerEnum::CREATURE, idCreature );
}

bool WorldCombatSystem::resolveCharacterAttack( int idCharacter, int x, int y, int z, double damage ) {
    int idCreature = NO_CREATURE;
    bool creatureDied = false;
    double creatureHealth = 0.0;
    double creatureMaxHealth = 0.0;
    double creatureMovementCooldownSeconds = 0.0;

    {
        std::lock_guard<std::mutex> lock( _runtime.mutex() );

        if ( !_runtime.characterLocked( idCharacter ) ) {
            return false;
        }

        Engine::CreatureModel* creaturePtr = _runtime.spatialIndex().creatureAt( x, y, z );

        if ( creaturePtr ) {
            idCreature = creaturePtr->idCreature();

            creaturePtr->vitals().setHealth( std::max( 0.0, creaturePtr->vitals().health() - damage ) );

            creatureDied = creaturePtr->vitals().health() <= 0.0;
            creatureHealth = creaturePtr->vitals().health();
            creatureMaxHealth = creaturePtr->vitals().maxHealth();
            creatureMovementCooldownSeconds = creaturePtr->movement().cooldownSeconds();

            if ( creatureDied ) {
                _runtime.eraseCreatureLocked( idCreature );
            }
        }
    }

    EventBus& eventBus = _runtime.eventBus();

    Json::Value attackPayload;
    attackPayload[ "idCharacter" ] = idCharacter;
    attackPayload[ "x" ] = x;
    attackPayload[ "y" ] = y;
    attackPayload[ "z" ] = z;
    eventBus.publish( WorldEvent( WorldEventType::CHARACTER_ATTACKED, attackPayload ) );

    if ( idCreature == NO_CREATURE ) {
        return true;
    }

    Json::Value creaturePayload;
    creaturePayload[ "idCreature" ] = idCreature;
    creaturePayload[ "x" ] = x;
    creaturePayload[ "y" ] = y;
    creaturePayload[ "z" ] = z;
    creaturePayload[ "health" ] = creatureHealth;
    creaturePayload[ "maxHealth" ] = creatureMaxHealth;
    creaturePayload[ "movementCooldownSeconds" ] = creatureMovementCooldownSeconds;
    eventBus.publish( WorldEvent( creatureDied ? WorldEventType::CREATURE_LEFT : WorldEventType::CREATURE_VITALS_CHANGED, creaturePayload ) );

    return true;
}

void WorldCombatSystem::resolveCreatureAttack( int idCreature, int x, int y, int z, double damage ) {
    int idVictim = 0;
    bool hitCharacter = false;

    {
        std::lock_guard<std::mutex> lock( _runtime.mutex() );

        auto& creatures = _runtime.creaturesLocked();
        if ( creatures.find( idCreature ) == creatures.end() ) {
            return;
        }

        // TODO: morte de personagem; por ora a vida só é limitada a 0
        Engine::CharacterModel* victim = _runtime.spatialIndex().characterAt( x, y, z );

        if ( victim ) {
            victim->vitals().setHealth( std::max( 0.0, victim->vitals().health() - damage ) );
            idVictim = victim->idCharacter();
            hitCharacter = true;
        }
    }

    EventBus& eventBus = _runtime.eventBus();

    Json::Value attackPayload;
    attackPayload[ "idCreature" ] = idCreature;
    attackPayload[ "x" ] = x;
    attackPayload[ "y" ] = y;
    attackPayload[ "z" ] = z;
    eventBus.publish( WorldEvent( WorldEventType::CREATURE_ATTACKED, attackPayload ) );

    if ( !hitCharacter ) {
        return;
    }

    Json::Value vitalsPayload;
    vitalsPayload[ "idCharacter" ] = idVictim;
    eventBus.publish( WorldEvent( WorldEventType::CHARACTER_VITALS_CHANGED, vitalsPayload ) );
}

bool WorldCombatSystem::hasPendingAttack( WorldCombatAttackerEnum attacker, int idAttacker ) const {
    return std::any_of( _pendingAttacks.begin(), _pendingAttacks.end(), [ attacker, idAttacker ]( const PendingAttack& pendingAttack ) {
        return pendingAttack.attacker == attacker && pendingAttack.idAttacker == idAttacker;
    } );
}

int WorldCombatSystem::castTicks() const {
    return std::max( 1, static_cast<int>( std::ceil( ATTACK_CAST_SECONDS * _runtime.tickRate() ) ) );
}

} // namespace Server
