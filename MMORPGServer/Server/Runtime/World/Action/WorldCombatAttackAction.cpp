#include "WorldCombatAttackAction.h"

#include <algorithm>
#include <cmath>

#include <QDebug>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/Creature/CreatureModel.h>
#include <MMORPGServer/Server/Event/WorldEvent.h>
#include <MMORPGServer/Server/Event/WorldEventType.h>
#include <MMORPGServer/Server/Runtime/Character/CharacterAttackResolver.h>
#include <MMORPGServer/Server/Runtime/World/Action/WorldCombatAttackPlanner.h>
#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace {

constexpr double ATTACK_STAMINA_COST = 10.0;

} // namespace

namespace Server {

WorldCombatAttackAction::WorldCombatAttackAction( WorldRuntime& runtime ) :
    WorldCombatAction( runtime ) {
}

Engine::CombatActionEnum WorldCombatAttackAction::id() const {
    return Engine::CombatActionEnum::ATTACK;
}

void WorldCombatAttackAction::onTick() {
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
            resolveCharacterStrike( pendingAttack.idAttacker, pendingAttack.cells, pendingAttack.z, pendingAttack.hitMode, pendingAttack.damage );
        } else {
            resolveCreatureStrike( pendingAttack.idAttacker, pendingAttack.cells, pendingAttack.z, pendingAttack.hitMode, pendingAttack.damage );
        }
    }
}

bool WorldCombatAttackAction::start( WorldCombatAttackerEnum attacker, int idAttacker, int targetX, int targetY, int targetZ ) {
    if ( attacker == WorldCombatAttackerEnum::CHARACTER ) {
        return startCharacterAttack( idAttacker, targetX, targetY, targetZ );
    }

    return startCreatureAttack( idAttacker, targetX, targetY, targetZ );
}

void WorldCombatAttackAction::cancel( WorldCombatAttackerEnum attacker, int idAttacker ) {
    _pendingAttacks.erase( std::remove_if( _pendingAttacks.begin(), _pendingAttacks.end(), [ attacker, idAttacker ]( const PendingAttack& pendingAttack ) {
                               return pendingAttack.attacker == attacker && pendingAttack.idAttacker == idAttacker;
                           } ),
                           _pendingAttacks.end() );
}

bool WorldCombatAttackAction::isActive( WorldCombatAttackerEnum attacker, int idAttacker ) const {
    return std::any_of( _pendingAttacks.begin(), _pendingAttacks.end(), [ attacker, idAttacker ]( const PendingAttack& pendingAttack ) {
        return pendingAttack.attacker == attacker && pendingAttack.idAttacker == idAttacker;
    } );
}

bool WorldCombatAttackAction::startCharacterAttack( int idCharacter, int targetX, int targetY, int targetZ ) {
    Engine::CharacterModel* character = _runtime.character( idCharacter );
    if ( !character ) {
        return false;
    }

    const bool available = character->combat().isActionAvailable( id() );
    const bool hasStamina = character->vitals().stamina() >= ATTACK_STAMINA_COST;
    const bool attackDue = character->combat().isReady( _runtime.tickRate() );
    const bool casting = isActive( WorldCombatAttackerEnum::CHARACTER, idCharacter );

    if ( !available || !hasStamina || !attackDue || casting ) {
        qInfo() << "[WorldCombatAttackAction] Attack blocked [CHARACTER]" << idCharacter << "[AVAILABLE]" << available << "[HAS_STAMINA]" << hasStamina
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

    const std::vector<Engine::CharacterEquipmentModel> equipment = character->equipment();
    const Engine::AttackModel attack = CharacterAttackResolver::attack( equipment );
    const double damage = CharacterAttackResolver::damage( equipment );
    const Engine::EntityPositionModel position = character->position();

    const std::vector<WorldCombatStrikeModel> strikes = WorldCombatAttackPlanner::plan( attack, position.x(), position.y(), targetX - position.x(), targetY - position.y(), _runtime.tickRate() );

    Json::Value planPayload( Json::arrayValue );

    for ( const WorldCombatStrikeModel& strike : strikes ) {
        _pendingAttacks.push_back( PendingAttack( WorldCombatAttackerEnum::CHARACTER, idCharacter, strike.cells(), targetZ, strike.hitMode(), damage * strike.damageMultiplier(), strike.ticks() ) );
        planPayload.append( strikePayload( strike ) );
    }

    Json::Value startedPayload;
    startedPayload[ "idCharacter" ] = idCharacter;
    startedPayload[ "x" ] = targetX;
    startedPayload[ "y" ] = targetY;
    startedPayload[ "z" ] = targetZ;
    startedPayload[ "castSeconds" ] = attack.castSeconds();
    startedPayload[ "plan" ] = planPayload;
    _runtime.eventBus().publish( WorldEvent( WorldEventType::CHARACTER_ATTACK_STARTED, startedPayload ) );

    Json::Value vitalsPayload;
    vitalsPayload[ "idCharacter" ] = idCharacter;
    _runtime.eventBus().publish( WorldEvent( WorldEventType::CHARACTER_VITALS_CHANGED, vitalsPayload ) );

    qInfo() << "[WorldCombatAttackAction] Character started attack [CHARACTER]" << idCharacter << "[X]" << targetX << "[Y]" << targetY << "[Z]" << targetZ;

    return true;
}

bool WorldCombatAttackAction::startCreatureAttack( int idCreature, int targetX, int targetY, int targetZ ) {
    Engine::EntityPositionModel position;
    QString creatureTypeKey;

    {
        std::lock_guard<std::mutex> lock( _runtime.mutex() );

        auto& creatures = _runtime.creaturesLocked();
        auto it = creatures.find( idCreature );
        if ( it == creatures.end() ) {
            return false;
        }

        it->second->creature()->combat().setCounter( 0 );
        position = it->second->creature()->position();
        creatureTypeKey = it->second->creature()->type();
    }

    const Engine::CreatureTypeModel* creatureType = Engine::Singleton<Engine::DataManager>::instance().creatureTypeCatalog().creatureType( creatureTypeKey );

    const Engine::AttackModel attack = creatureType && creatureType->attack().has_value() ? creatureType->attack().value() : Engine::AttackModel::basic();
    const double damage = creatureType ? creatureType->damage() : 0.0;

    const std::vector<WorldCombatStrikeModel> strikes = WorldCombatAttackPlanner::plan( attack, position.x(), position.y(), targetX - position.x(), targetY - position.y(), _runtime.tickRate() );

    Json::Value planPayload( Json::arrayValue );

    for ( const WorldCombatStrikeModel& strike : strikes ) {
        _pendingAttacks.push_back( PendingAttack( WorldCombatAttackerEnum::CREATURE, idCreature, strike.cells(), targetZ, strike.hitMode(), damage * strike.damageMultiplier(), strike.ticks() ) );
        planPayload.append( strikePayload( strike ) );
    }

    Json::Value startedPayload;
    startedPayload[ "idCreature" ] = idCreature;
    startedPayload[ "x" ] = targetX;
    startedPayload[ "y" ] = targetY;
    startedPayload[ "z" ] = targetZ;
    startedPayload[ "castSeconds" ] = attack.castSeconds();
    startedPayload[ "plan" ] = planPayload;
    _runtime.eventBus().publish( WorldEvent( WorldEventType::CREATURE_ATTACK_STARTED, startedPayload ) );

    return true;
}

Json::Value WorldCombatAttackAction::cellsPayload( const std::vector<std::pair<int, int>>& cells ) {
    Json::Value payload( Json::arrayValue );

    for ( const std::pair<int, int>& cell : cells ) {
        Json::Value cellPayload;
        cellPayload[ "x" ] = cell.first;
        cellPayload[ "y" ] = cell.second;
        payload.append( cellPayload );
    }

    return payload;
}

Json::Value WorldCombatAttackAction::strikePayload( const WorldCombatStrikeModel& strike ) {
    Json::Value payload;
    payload[ "seconds" ] = strike.seconds();
    payload[ "cells" ] = cellsPayload( strike.cells() );

    return payload;
}

bool WorldCombatAttackAction::resolveCharacterAttack( int idCharacter, int x, int y, int z, double damage ) {
    return resolveCharacterStrike( idCharacter, { std::make_pair( x, y ) }, z, Engine::AttackHitModeEnum::FIRST, damage );
}

bool WorldCombatAttackAction::resolveCharacterStrike( int idCharacter, const std::vector<std::pair<int, int>>& cells, int z, Engine::AttackHitModeEnum hitMode, double damage ) {
    std::vector<Json::Value> creaturePayloads;
    std::vector<bool> creatureDeaths;

    {
        std::lock_guard<std::mutex> lock( _runtime.mutex() );

        if ( !_runtime.characterLocked( idCharacter ) ) {
            return false;
        }

        std::vector<int> hitCreatures;

        for ( const std::pair<int, int>& cell : cells ) {
            Engine::CreatureModel* creaturePtr = _runtime.spatialIndex().creatureAt( cell.first, cell.second, z );

            if ( !creaturePtr ) {
                continue;
            }

            const int idCreature = creaturePtr->idCreature();

            if ( std::find( hitCreatures.begin(), hitCreatures.end(), idCreature ) != hitCreatures.end() ) {
                continue;
            }

            hitCreatures.push_back( idCreature );

            creaturePtr->vitals().setHealth( std::max( 0.0, creaturePtr->vitals().health() - damage ) );

            const bool creatureDied = creaturePtr->vitals().health() <= 0.0;

            Json::Value creaturePayload;
            creaturePayload[ "idCreature" ] = idCreature;
            creaturePayload[ "x" ] = creaturePtr->position().x();
            creaturePayload[ "y" ] = creaturePtr->position().y();
            creaturePayload[ "z" ] = z;
            creaturePayload[ "health" ] = creaturePtr->vitals().health();
            creaturePayload[ "maxHealth" ] = creaturePtr->vitals().maxHealth();
            creaturePayload[ "movementCooldownSeconds" ] = creaturePtr->movement().cooldownSeconds();
            creaturePayload[ "footprint" ] = creaturePtr->size().width();
            creaturePayload[ "creatureType" ] = creaturePtr->type().toStdString();

            creaturePayloads.push_back( creaturePayload );
            creatureDeaths.push_back( creatureDied );

            if ( creatureDied ) {
                _runtime.eraseCreatureLocked( idCreature );
            }

            if ( hitMode == Engine::AttackHitModeEnum::FIRST ) {
                break;
            }
        }
    }

    EventBus& eventBus = _runtime.eventBus();

    Json::Value attackPayload;
    attackPayload[ "idCharacter" ] = idCharacter;
    attackPayload[ "x" ] = cells.front().first;
    attackPayload[ "y" ] = cells.front().second;
    attackPayload[ "z" ] = z;
    attackPayload[ "cells" ] = cellsPayload( cells );
    eventBus.publish( WorldEvent( WorldEventType::CHARACTER_ATTACKED, attackPayload ) );

    for ( size_t index = 0; index < creaturePayloads.size(); ++index ) {
        Json::Value& creaturePayload = creaturePayloads[ index ];
        const bool creatureDied = creatureDeaths[ index ];

        eventBus.publish( WorldEvent( creatureDied ? WorldEventType::CREATURE_LEFT : WorldEventType::CREATURE_VITALS_CHANGED, creaturePayload ) );

        if ( creatureDied ) {
            creaturePayload[ "type" ] = creaturePayload[ "creatureType" ];
            eventBus.publish( WorldEvent( WorldEventType::CREATURE_DIED, creaturePayload ) );
        }
    }

    return true;
}

void WorldCombatAttackAction::resolveCreatureAttack( int idCreature, int x, int y, int z, double damage ) {
    resolveCreatureStrike( idCreature, { std::make_pair( x, y ) }, z, Engine::AttackHitModeEnum::FIRST, damage );
}

void WorldCombatAttackAction::resolveCreatureStrike( int idCreature, const std::vector<std::pair<int, int>>& cells, int z, Engine::AttackHitModeEnum hitMode, double damage ) {
    std::vector<int> victims;

    {
        std::lock_guard<std::mutex> lock( _runtime.mutex() );

        auto& creatures = _runtime.creaturesLocked();
        if ( creatures.find( idCreature ) == creatures.end() ) {
            return;
        }

        for ( const std::pair<int, int>& cell : cells ) {
            // TODO: morte de personagem; por ora a vida só é limitada a 0
            Engine::CharacterModel* victim = _runtime.spatialIndex().characterAt( cell.first, cell.second, z );

            if ( !victim || std::find( victims.begin(), victims.end(), victim->idCharacter() ) != victims.end() ) {
                continue;
            }

            victim->vitals().setHealth( std::max( 0.0, victim->vitals().health() - damage ) );
            victims.push_back( victim->idCharacter() );

            if ( hitMode == Engine::AttackHitModeEnum::FIRST ) {
                break;
            }
        }
    }

    EventBus& eventBus = _runtime.eventBus();

    Json::Value attackPayload;
    attackPayload[ "idCreature" ] = idCreature;
    attackPayload[ "x" ] = cells.front().first;
    attackPayload[ "y" ] = cells.front().second;
    attackPayload[ "z" ] = z;
    attackPayload[ "cells" ] = cellsPayload( cells );
    eventBus.publish( WorldEvent( WorldEventType::CREATURE_ATTACKED, attackPayload ) );

    for ( int idVictim : victims ) {
        Json::Value vitalsPayload;
        vitalsPayload[ "idCharacter" ] = idVictim;
        eventBus.publish( WorldEvent( WorldEventType::CHARACTER_VITALS_CHANGED, vitalsPayload ) );
    }
}

} // namespace Server
