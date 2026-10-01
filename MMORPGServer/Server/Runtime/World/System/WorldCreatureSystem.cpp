#include "WorldCreatureSystem.h"

#include <cstdlib>
#include <vector>

#include <json/json.h>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Creature/CreatureTypeModel.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGServer/Server/Event/WorldEvent.h>
#include <MMORPGServer/Server/Event/WorldEventType.h>
#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace {

class CreatureAttackIntent {
public:
    CreatureAttackIntent( int idCreature, int x, int y, int z ) :
        idCreature( idCreature ),
        x( x ),
        y( y ),
        z( z ) {
    }

    int idCreature;
    int x;
    int y;
    int z;
};

bool isAdjacent( const Engine::EntityPositionModel& from, const Engine::EntityPositionModel& to ) {
    return from.z() == to.z() && std::abs( from.x() - to.x() ) <= 1 && std::abs( from.y() - to.y() ) <= 1;
}

} // namespace

namespace Server {

WorldCreatureSystem::WorldCreatureSystem( WorldRuntime& runtime ) :
    WorldSystem( runtime ) {
}

void WorldCreatureSystem::onTick() {
    std::vector<Json::Value> movedCreaturePayloads;
    std::vector<CreatureAttackIntent> attackIntents;

    {
        std::lock_guard<std::mutex> lock( _runtime.mutex() );

        const WorldSpatialIndex& spatialIndex = _runtime.spatialIndex();
        const Engine::CreatureTypeCatalog& creatureTypeCatalog = Engine::Singleton<Engine::DataManager>::instance().creatureTypeCatalog();

        for ( auto& entry : _runtime.creaturesLocked() ) {
            CreatureRuntime& creatureRuntime = *entry.second;
            Engine::CreatureModel* creaturePtr = creatureRuntime.creature();

            Engine::EntityCombatModel& combat = creaturePtr->combat();
            combat.setCounter( combat.counter() + 1 );

            const Engine::CreatureTypeModel* creatureType = creatureTypeCatalog.creatureType( creaturePtr->type() );
            const int aggroRadius = creatureType ? static_cast<int>( creatureType->aggroRadius() ) : 0;
            const Engine::CharacterModel* target = aggroRadius > 0 ? spatialIndex.nearestCharacterWithin( creaturePtr->position(), aggroRadius ) : nullptr;

            if ( target ) {
                // TODO: perseguir o alvo com A*; por ora a criatura com agro fica parada e só ataca quando adjacente
                const bool canAttack = isAdjacent( creaturePtr->position(), target->position() ) && combat.isReady( _runtime.tickRate() ) && !_runtime.combatSystem().hasPendingCreatureAttack( creaturePtr->idCreature() );

                if ( canAttack ) {
                    attackIntents.push_back( CreatureAttackIntent( creaturePtr->idCreature(), target->position().x(), target->position().y(), target->position().z() ) );
                }

                continue;
            }

            const auto movedPosition = creatureRuntime.tick(
                [ &spatialIndex ]( const Engine::EntityPositionModel& position ) { return spatialIndex.hasCharacterNear( position ); },
                [ &spatialIndex ]( const Engine::EntityPositionModel& position ) { return spatialIndex.isPositionOccupied( position ); },
                _runtime.tickRate() );

            if ( !movedPosition ) {
                continue;
            }

            Json::Value payload;
            payload[ "idCreature" ] = creaturePtr->idCreature();
            payload[ "x" ] = movedPosition->x();
            payload[ "y" ] = movedPosition->y();
            payload[ "z" ] = movedPosition->z();
            payload[ "health" ] = creaturePtr->vitals().health();
            payload[ "maxHealth" ] = creaturePtr->vitals().maxHealth();
            payload[ "movementCooldownSeconds" ] = creaturePtr->movement().cooldownSeconds();

            movedCreaturePayloads.push_back( payload );
        }
    }

    for ( const Json::Value& payload : movedCreaturePayloads ) {
        _runtime.eventBus().publish( WorldEvent( WorldEventType::CREATURE_MOVED, payload ) );
    }

    for ( const CreatureAttackIntent& intent : attackIntents ) {
        _runtime.combatSystem().attackCreature( intent.idCreature, intent.x, intent.y, intent.z );
    }
}

} // namespace Server
