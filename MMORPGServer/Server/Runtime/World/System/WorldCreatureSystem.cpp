#include "WorldCreatureSystem.h"

#include <cstdlib>
#include <optional>
#include <vector>

#include <json/json.h>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Creature/CreatureTypeModel.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/World/WorldModel.h>
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

// TODO: ranged attackers also need line of sight, not only distance
bool isWithinRange( const Engine::EntityPositionModel& from, const Engine::EntityPositionModel& to, int range ) {
    return from.z() == to.z() && std::abs( from.x() - to.x() ) <= range && std::abs( from.y() - to.y() ) <= range;
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
        const Engine::WorldModel* world = _runtime.world();

        const auto isWalkable = [ world ]( int x, int y, int z ) {
            const Engine::WorldTileModel* worldTile = world ? world->tile( x, y, z ) : nullptr;
            return worldTile && worldTile->tileModel() && worldTile->tileModel()->isWalkable();
        };

        const auto isOccupied = [ &spatialIndex ]( int x, int y, int z ) {
            Engine::EntityPositionModel position;
            position.setX( x );
            position.setY( y );
            position.setZ( z );
            return spatialIndex.isPositionOccupied( position );
        };

        for ( auto& entry : _runtime.creaturesLocked() ) {
            CreatureRuntime& creatureRuntime = *entry.second;
            Engine::CreatureModel* creaturePtr = creatureRuntime.creature();

            Engine::EntityCombatModel& combat = creaturePtr->combat();
            combat.setCounter( combat.counter() + 1 );

            creatureRuntime.updateLeash();

            const Engine::CreatureTypeModel* creatureType = creatureTypeCatalog.creatureType( creaturePtr->type() );
            const int aggroRadius = creatureType ? static_cast<int>( creatureType->aggroRadius() ) : 0;
            const Engine::CharacterModel* target = aggroRadius > 0 && !creatureRuntime.isReturning() ? spatialIndex.nearestCharacterWithin( creaturePtr->position(), aggroRadius ) : nullptr;

            std::optional<Engine::EntityPositionModel> movedPosition;

            if ( target ) {
                const int attackRange = combat.attackRange();

                if ( !isWithinRange( creaturePtr->position(), target->position(), attackRange ) ) {
                    movedPosition = creatureRuntime.chase( target->position(), attackRange, isWalkable, isOccupied, _runtime.tickRate() );
                } else if ( combat.isReady( _runtime.tickRate() ) && !_runtime.combatSystem().hasPendingCreatureAttack( creaturePtr->idCreature() ) ) {
                    attackIntents.push_back( CreatureAttackIntent( creaturePtr->idCreature(), target->position().x(), target->position().y(), target->position().z() ) );
                }
            } else {
                movedPosition = creatureRuntime.tick(
                    [ &spatialIndex ]( const Engine::EntityPositionModel& position ) { return spatialIndex.hasCharacterNear( position ); },
                    isWalkable,
                    isOccupied,
                    _runtime.tickRate() );
            }

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
