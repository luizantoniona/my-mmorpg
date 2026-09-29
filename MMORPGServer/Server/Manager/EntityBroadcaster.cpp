#include "EntityBroadcaster.h"

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/Character/CharacterDTO.h>
#include <MMORPGEngine/Entity/Character/OwnCharacterDTO.h>
#include <MMORPGEngine/Entity/Character/OwnEquipmentDTO.h>
#include <MMORPGEngine/Entity/Character/OwnInventoryDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureLeftDTO.h>
#include <MMORPGEngine/Entity/EntityAttackedDTO.h>
#include <MMORPGEngine/Entity/EntityLeftDTO.h>
#include <MMORPGEngine/World/WorldBasicDTO.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>
#include <MMORPGServer/Server/Network/Observer/ObserverRegistry.h>

namespace Server {

EntityBroadcaster::EntityBroadcaster() {
    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    worldRuntime.eventBus().subscribe( WorldEventType::ENTITY_ENTERED, [ this ]( const WorldEvent& event ) {
        onEntityEntered( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::ENTITY_MOVED, [ this ]( const WorldEvent& event ) {
        onEntityMoved( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::ENTITY_LEFT, [ this ]( const WorldEvent& event ) {
        onEntityLeft( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::ENTITY_VITALS_CHANGED, [ this ]( const WorldEvent& event ) {
        onEntityVitalsChanged( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::ENTITY_ATTACKED, [ this ]( const WorldEvent& event ) {
        onEntityAttacked( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::CREATURE_MOVED, [ this ]( const WorldEvent& event ) {
        onCreatureMoved( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::CREATURE_VITALS_CHANGED, [ this ]( const WorldEvent& event ) {
        onCreatureVitalsChanged( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::CREATURE_LEFT, [ this ]( const WorldEvent& event ) {
        onCreatureLeft( event );
    } );
}

void EntityBroadcaster::sendSnapshot( EntityObserver& observer ) {
    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    for ( const Engine::CharacterModel& character : worldRuntime.connectedCharacters() ) {
        observer.send( Engine::JsonHelper::writeJsonString( Engine::CharacterDTO::fromModel( &character ).toJson() ) );
    }

    for ( const Engine::CreatureModel& creature : worldRuntime.creatures() ) {
        observer.send( Engine::JsonHelper::writeJsonString( Engine::CreatureDTO::fromModel( &creature ).toJson() ) );
    }
}

void EntityBroadcaster::onEntityEntered( const WorldEvent& event ) {
    sendWorldBasic( event );
    sendOwnCharacter( event );
    sendOwnEquipment( event );
    sendOwnInventory( event );
    sendNearbyCharacters( event );
    sendCreatures( event );
    broadcastCharacter( event );
}

void EntityBroadcaster::onEntityMoved( const WorldEvent& event ) {
    broadcastCharacter( event );
}

void EntityBroadcaster::onEntityLeft( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    Engine::EntityLeftDTO message;
    message.setIdCharacter( payload[ "idCharacter" ].asInt() );

    std::vector<int> receivers;

    for ( const Json::Value& nearbyIdCharacter : payload[ "nearby" ] ) {
        receivers.push_back( nearbyIdCharacter.asInt() );
    }

    broadcast( receivers, Engine::JsonHelper::writeJsonString( message.toJson() ) );
}

void EntityBroadcaster::onEntityVitalsChanged( const WorldEvent& event ) {
    sendOwnCharacter( event );
    broadcastCharacter( event );
}

void EntityBroadcaster::onEntityAttacked( const WorldEvent& event ) {
    broadcastAttack( event );
}

void EntityBroadcaster::onCreatureMoved( const WorldEvent& event ) {
    broadcastCreature( event.payload()[ "idCreature" ].asInt() );
}

void EntityBroadcaster::onCreatureVitalsChanged( const WorldEvent& event ) {
    broadcastCreature( event.payload()[ "idCreature" ].asInt() );
}

void EntityBroadcaster::onCreatureLeft( const WorldEvent& event ) {
    Engine::CreatureLeftDTO message;
    message.setIdCreature( event.payload()[ "idCreature" ].asInt() );

    broadcastToConnected( Engine::JsonHelper::writeJsonString( message.toJson() ) );
}

void EntityBroadcaster::sendWorldBasic( const WorldEvent& event ) {
    const Engine::WorldModel* world = Engine::Singleton<WorldManager>::instance().runtime().world();
    if ( !world ) {
        return;
    }

    sendToCharacter( event.payload()[ "idCharacter" ].asInt(), Engine::JsonHelper::writeJsonString( Engine::WorldBasicDTO::fromModel( world ).toJson() ) );
}

void EntityBroadcaster::sendOwnCharacter( const WorldEvent& event ) {
    const int idCharacter = event.payload()[ "idCharacter" ].asInt();

    const Engine::CharacterModel* character = Engine::Singleton<WorldManager>::instance().runtime().character( idCharacter );
    if ( !character ) {
        return;
    }

    sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( Engine::OwnCharacterDTO::fromModel( character ).toJson() ) );
}

void EntityBroadcaster::sendOwnEquipment( const WorldEvent& event ) {
    const int idCharacter = event.payload()[ "idCharacter" ].asInt();

    const Engine::CharacterModel* character = Engine::Singleton<WorldManager>::instance().runtime().character( idCharacter );
    if ( !character ) {
        return;
    }

    sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( Engine::OwnEquipmentDTO::fromModel( character ).toJson() ) );
}

void EntityBroadcaster::sendOwnInventory( const WorldEvent& event ) {
    const int idCharacter = event.payload()[ "idCharacter" ].asInt();

    const Engine::CharacterModel* character = Engine::Singleton<WorldManager>::instance().runtime().character( idCharacter );
    if ( !character ) {
        return;
    }

    sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( Engine::OwnInventoryDTO::fromModel( character ).toJson() ) );
}

void EntityBroadcaster::sendNearbyCharacters( const WorldEvent& event ) {
    const int idCharacter = event.payload()[ "idCharacter" ].asInt();

    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    for ( int nearbyIdCharacter : worldRuntime.charactersNear( idCharacter ) ) {
        const Engine::CharacterModel* nearbyCharacter = worldRuntime.character( nearbyIdCharacter );
        if ( !nearbyCharacter ) {
            continue;
        }

        sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( Engine::CharacterDTO::fromModel( nearbyCharacter ).toJson() ) );
    }
}

void EntityBroadcaster::sendCreatures( const WorldEvent& event ) {
    const int idCharacter = event.payload()[ "idCharacter" ].asInt();

    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    // TODO: Filter by proximity once creatures move/spawn dynamically (Backlog "Criaturas")
    for ( const Engine::CreatureModel& creature : worldRuntime.creatures() ) {
        sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( Engine::CreatureDTO::fromModel( &creature ).toJson() ) );
    }
}

void EntityBroadcaster::broadcastCharacter( const WorldEvent& event ) {
    const int idCharacter = event.payload()[ "idCharacter" ].asInt();

    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    const Engine::CharacterModel* character = worldRuntime.character( idCharacter );
    if ( !character ) {
        return;
    }

    broadcast( worldRuntime.charactersNear( idCharacter ), Engine::JsonHelper::writeJsonString( Engine::CharacterDTO::fromModel( character ).toJson() ) );
}

void EntityBroadcaster::broadcastAttack( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    const int idCharacter = payload[ "idCharacter" ].asInt();

    Engine::EntityAttackedDTO message;
    message.setIdCharacter( idCharacter );
    message.setX( payload[ "x" ].asInt() );
    message.setY( payload[ "y" ].asInt() );
    message.setZ( payload[ "z" ].asInt() );

    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    std::vector<int> receivers = worldRuntime.charactersNear( idCharacter );
    receivers.push_back( idCharacter );

    broadcast( receivers, Engine::JsonHelper::writeJsonString( message.toJson() ) );
}

void EntityBroadcaster::broadcastCreature( int idCreature ) {
    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    const Engine::CreatureModel* creature = worldRuntime.creature( idCreature );
    if ( !creature ) {
        return;
    }

    // TODO: Filter by proximity once creatures need to scale beyond a handful (Backlog "Criaturas")
    broadcastToConnected( Engine::JsonHelper::writeJsonString( Engine::CreatureDTO::fromModel( creature ).toJson() ) );
}

void EntityBroadcaster::sendToCharacter( int idCharacter, const std::string& message ) {
    std::shared_ptr<EntityObserver> observer = Engine::Singleton<ObserverRegistry>::instance().characterObserver( idCharacter );

    if ( observer ) {
        observer->send( message );
    }
}

void EntityBroadcaster::broadcast( const std::vector<int>& idCharacters, const std::string& message ) {
    auto& observerRegistry = Engine::Singleton<ObserverRegistry>::instance();

    for ( int idCharacter : idCharacters ) {
        std::shared_ptr<EntityObserver> observer = observerRegistry.characterObserver( idCharacter );

        if ( observer ) {
            observer->send( message );
        }
    }

    for ( const std::shared_ptr<EntityObserver>& observer : observerRegistry.globalObservers() ) {
        observer->send( message );
    }
}

void EntityBroadcaster::broadcastToConnected( const std::string& message ) {
    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    std::vector<int> receivers;

    for ( const Engine::CharacterModel& character : worldRuntime.connectedCharacters() ) {
        receivers.push_back( character.idCharacter() );
    }

    broadcast( receivers, message );
}

} // namespace Server
