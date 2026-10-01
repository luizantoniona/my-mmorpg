#include "WorldSpawnSystem.h"

#include <random>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Creature/CreatureTypeModel.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Entity/Creature/CreatureModel.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace Server {

WorldSpawnSystem::WorldSpawnSystem( WorldRuntime& runtime ) :
    WorldSystem( runtime ),
    _nextIdCreature( 1 ) {
}

void WorldSpawnSystem::onTick() {
}

void WorldSpawnSystem::spawnCreaturesFromAreas() {
    Engine::WorldModel* world = _runtime.world();
    if ( !world ) {
        return;
    }

    std::mt19937 randomEngine( std::random_device{}() );

    for ( int z : world->floors() ) {
        for ( const Engine::CreatureSpawnAreaModel& area : world->spawnAreas( z ) ) {
            if ( area.width() == 0 || area.height() == 0 ) {
                continue;
            }

            std::uniform_int_distribution<int> xDistribution( area.x(), area.x() + static_cast<int>( area.width() ) - 1 );
            std::uniform_int_distribution<int> yDistribution( area.y(), area.y() + static_cast<int>( area.height() ) - 1 );

            for ( const Engine::CreatureSpawnEntryModel& entry : area.creatures() ) {
                const Engine::CreatureTypeModel* creatureType = Engine::Singleton<Engine::DataManager>::instance().creatureTypeCatalog().creatureType( entry.type() );

                for ( uint32_t i = 0; i < entry.quantity(); ++i ) {
                    auto creature = std::make_unique<Engine::CreatureModel>();
                    creature->setIdCreature( _nextIdCreature++ );
                    creature->setType( entry.type() );
                    creature->position().setX( xDistribution( randomEngine ) );
                    creature->position().setY( yDistribution( randomEngine ) );
                    creature->position().setZ( z );

                    if ( creatureType ) {
                        creature->vitals().setMaxHealth( creatureType->vitals().maxHealth() );
                        creature->vitals().setHealth( creatureType->vitals().maxHealth() );
                    }

                    _runtime.addCreature( std::move( creature ) );
                }
            }
        }
    }
}

} // namespace Server
