#include "WorldSpawnSystem.h"

#include <algorithm>
#include <cmath>

#include <QDebug>

#include <json/json.h>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Creature/CreatureTypeModel.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Entity/Creature/CreatureModel.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Event/WorldEventType.h>
#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace {

constexpr int MAX_SPAWN_ATTEMPTS = 20;

} // namespace

namespace Server {

WorldSpawnSystem::WorldSpawnSystem( WorldRuntime& runtime ) :
    WorldSystem( runtime ),
    _randomEngine( std::random_device{}() ),
    _currentTick( 0 ),
    _nextIdCreature( 1 ),
    _areasBuilt( false ) {

    _runtime.eventBus().subscribe( WorldEventType::CREATURE_DIED, [ this ]( const WorldEvent& event ) {
        onCreatureDied( event );
    } );
}

void WorldSpawnSystem::onTick() {
    ++_currentTick;

    if ( !_areasBuilt ) {
        buildAreas();
    }

    for ( size_t i = 0; i < _activeAreas.size(); ++i ) {
        spawnDue( *_activeAreas[ i ] );
    }

    _activeAreas.erase( std::remove_if( _activeAreas.begin(), _activeAreas.end(), []( const WorldSpawnAreaRuntime* area ) {
                            return !area->hasPending();
                        } ),
                        _activeAreas.end() );
}

const std::vector<std::unique_ptr<WorldSpawnAreaRuntime>>& WorldSpawnSystem::areas() const {
    return _areas;
}

size_t WorldSpawnSystem::activeAreaCount() const {
    return _activeAreas.size();
}

void WorldSpawnSystem::buildAreas() {
    _areasBuilt = true;

    const Engine::WorldModel* world = _runtime.world();
    if ( !world ) {
        return;
    }

    for ( int z : world->floors() ) {
        for ( const Engine::CreatureSpawnAreaModel& areaModel : world->spawnAreas( z ) ) {
            if ( areaModel.width() == 0 || areaModel.height() == 0 ) {
                continue;
            }

            if ( areaModel.respawnSeconds() <= 0.0 ) {
                qWarning() << "[WorldSpawnSystem] Spawn area without RespawnSeconds, skipped [X]" << areaModel.x() << "[Y]" << areaModel.y() << "[Z]" << z;
                continue;
            }

            auto area = std::make_unique<WorldSpawnAreaRuntime>( areaModel, z );

            for ( const Engine::CreatureSpawnEntryModel& entry : areaModel.creatures() ) {
                for ( uint32_t i = 0; i < entry.quantity(); ++i ) {
                    area->addPending( entry.type(), _currentTick );
                }
            }

            if ( area->hasPending() ) {
                _activeAreas.push_back( area.get() );
            }

            _areas.push_back( std::move( area ) );
        }
    }
}

void WorldSpawnSystem::onCreatureDied( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    const auto it = _creatureAreas.find( payload[ "idCreature" ].asInt() );
    if ( it == _creatureAreas.end() ) {
        return;
    }

    WorldSpawnAreaRuntime* area = it->second;
    _creatureAreas.erase( it );

    area->addPending( payload[ "type" ].asUInt(), _currentTick + ticksFromSeconds( area->area().respawnSeconds() ) );

    if ( std::find( _activeAreas.begin(), _activeAreas.end(), area ) == _activeAreas.end() ) {
        _activeAreas.push_back( area );
    }
}

void WorldSpawnSystem::spawnDue( WorldSpawnAreaRuntime& area ) {
    std::vector<WorldSpawnAreaRuntime::Pending>& pending = area.pending();

    for ( size_t i = 0; i < pending.size(); ) {
        if ( pending[ i ].dueTick > _currentTick ) {
            ++i;
            continue;
        }

        if ( spawnCreature( area, pending[ i ].type ) ) {
            pending.erase( pending.begin() + i );
        } else {
            pending[ i ].dueTick = _currentTick + ticksFromSeconds( 1.0 );
            ++i;
        }
    }
}

bool WorldSpawnSystem::spawnCreature( WorldSpawnAreaRuntime& area, uint32_t type ) {
    const Engine::WorldModel* world = _runtime.world();
    const Engine::CreatureSpawnAreaModel& areaModel = area.area();

    std::uniform_int_distribution<int> xDistribution( areaModel.x(), areaModel.x() + static_cast<int>( areaModel.width() ) - 1 );
    std::uniform_int_distribution<int> yDistribution( areaModel.y(), areaModel.y() + static_cast<int>( areaModel.height() ) - 1 );

    for ( int attempt = 0; attempt < MAX_SPAWN_ATTEMPTS; ++attempt ) {
        const int x = xDistribution( _randomEngine );
        const int y = yDistribution( _randomEngine );

        const Engine::WorldTileModel* worldTile = world->tile( x, y, area.z() );
        const bool walkable = worldTile && worldTile->tileModel() && worldTile->tileModel()->isWalkable();

        if ( !walkable || _runtime.isPositionOccupied( x, y, area.z() ) ) {
            continue;
        }

        auto creature = std::make_unique<Engine::CreatureModel>();
        creature->setIdCreature( _nextIdCreature++ );
        creature->setType( type );
        creature->position().setX( x );
        creature->position().setY( y );
        creature->position().setZ( area.z() );

        const Engine::CreatureTypeModel* creatureType = Engine::Singleton<Engine::DataManager>::instance().creatureTypeCatalog().creatureType( type );
        if ( creatureType ) {
            creature->vitals().setMaxHealth( creatureType->vitals().maxHealth() );
            creature->vitals().setHealth( creatureType->vitals().maxHealth() );
            creature->combat().setAttackRange( static_cast<int>( creatureType->attackRange() ) );
        }

        const Engine::CreatureModel* creaturePtr = _runtime.addCreature( std::move( creature ), &area );
        _creatureAreas[ creaturePtr->idCreature() ] = &area;

        Json::Value payload;
        payload[ "idCreature" ] = creaturePtr->idCreature();
        payload[ "x" ] = x;
        payload[ "y" ] = y;
        payload[ "z" ] = area.z();
        payload[ "health" ] = creaturePtr->vitals().health();
        payload[ "maxHealth" ] = creaturePtr->vitals().maxHealth();
        payload[ "movementCooldownSeconds" ] = creaturePtr->movement().cooldownSeconds();
        _runtime.eventBus().publish( WorldEvent( WorldEventType::CREATURE_ENTERED, payload ) );

        return true;
    }

    qWarning() << "[WorldSpawnSystem] No free walkable tile in spawn area, retrying [TYPE]" << type << "[Z]" << area.z();

    return false;
}

int64_t WorldSpawnSystem::ticksFromSeconds( double seconds ) const {
    return std::max<int64_t>( 1, std::llround( seconds * _runtime.tickRate() ) );
}

} // namespace Server
