#include "EntityBroadcaster.h"

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/Character/CharacterDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterEventAttackDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterEventAttackStartDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterEventLeaveDTO.h>
#include <MMORPGEngine/Entity/Character/OwnCharacterDTO.h>
#include <MMORPGEngine/Entity/Character/OwnEquipmentDTO.h>
#include <MMORPGEngine/Entity/Character/OwnInventoryDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureEventAttackDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureEventAttackStartDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureEventLeaveDTO.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/World/WorldBasicDTO.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>
#include <MMORPGServer/Server/Network/Observer/ObserverRegistry.h>

namespace Server {

EntityBroadcaster::EntityBroadcaster() {
    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    worldRuntime.eventBus().subscribe( WorldEventType::CHARACTER_ENTERED, [ this ]( const WorldEvent& event ) {
        onCharacterEntered( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::CHARACTER_MOVED, [ this ]( const WorldEvent& event ) {
        onCharacterMoved( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::CHARACTER_LEFT, [ this ]( const WorldEvent& event ) {
        onCharacterLeft( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::CHARACTER_VITALS_CHANGED, [ this ]( const WorldEvent& event ) {
        onCharacterVitalsChanged( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::CHARACTER_ATTACK_STARTED, [ this ]( const WorldEvent& event ) {
        onCharacterAttackStarted( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::CHARACTER_ATTACKED, [ this ]( const WorldEvent& event ) {
        onCharacterAttacked( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::CREATURE_MOVED, [ this ]( const WorldEvent& event ) {
        onCreatureMoved( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::CREATURE_VITALS_CHANGED, [ this ]( const WorldEvent& event ) {
        onCreatureVitalsChanged( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::CREATURE_ATTACK_STARTED, [ this ]( const WorldEvent& event ) {
        onCreatureAttackStarted( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::CREATURE_ATTACKED, [ this ]( const WorldEvent& event ) {
        onCreatureAttacked( event );
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

void EntityBroadcaster::onCharacterEntered( const WorldEvent& event ) {
    sendWorldBasic( event );
    sendOwnCharacter( event );
    sendOwnEquipment( event );
    sendOwnInventory( event );
    sendNearbyCharacters( event );
    sendCreatures( event );
    broadcastCharacter( event );
}

void EntityBroadcaster::onCharacterMoved( const WorldEvent& event ) {
    broadcastCharacter( event );
}

void EntityBroadcaster::onCharacterLeft( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    Engine::CharacterEventLeaveDTO message;
    message.setIdCharacter( payload[ "idCharacter" ].asInt() );

    std::vector<int> receivers;

    for ( const Json::Value& nearbyIdCharacter : payload[ "nearby" ] ) {
        receivers.push_back( nearbyIdCharacter.asInt() );
    }

    broadcast( receivers, Engine::JsonHelper::writeJsonString( message.toJson() ) );
}

void EntityBroadcaster::onCharacterVitalsChanged( const WorldEvent& event ) {
    sendOwnCharacter( event );
    broadcastCharacter( event );
}

void EntityBroadcaster::onCharacterAttackStarted( const WorldEvent& event ) {
    broadcastAttackStart( event );
}

void EntityBroadcaster::onCharacterAttacked( const WorldEvent& event ) {
    broadcastAttack( event );
}

void EntityBroadcaster::onCreatureMoved( const WorldEvent& event ) {
    broadcastCreature( event.payload() );
}

void EntityBroadcaster::onCreatureVitalsChanged( const WorldEvent& event ) {
    broadcastCreature( event.payload() );
}

void EntityBroadcaster::onCreatureLeft( const WorldEvent& event ) {
    Engine::CreatureEventLeaveDTO message;
    message.setIdCreature( event.payload()[ "idCreature" ].asInt() );

    broadcastNear( event.payload(), Engine::JsonHelper::writeJsonString( message.toJson() ) );
}

void EntityBroadcaster::onCreatureAttackStarted( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    Engine::CreatureEventAttackStartDTO message;
    message.setIdCreature( payload[ "idCreature" ].asInt() );
    message.setX( payload[ "x" ].asInt() );
    message.setY( payload[ "y" ].asInt() );
    message.setZ( payload[ "z" ].asInt() );
    message.setCastSeconds( payload[ "castSeconds" ].asDouble() );

    broadcastNear( payload, Engine::JsonHelper::writeJsonString( message.toJson() ) );
}

void EntityBroadcaster::onCreatureAttacked( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    Engine::CreatureEventAttackDTO message;
    message.setIdCreature( payload[ "idCreature" ].asInt() );
    message.setX( payload[ "x" ].asInt() );
    message.setY( payload[ "y" ].asInt() );
    message.setZ( payload[ "z" ].asInt() );

    broadcastNear( payload, Engine::JsonHelper::writeJsonString( message.toJson() ) );
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

    for ( const Engine::CreatureModel& creature : worldRuntime.creaturesNear( positionFromPayload( event.payload() ) ) ) {
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

void EntityBroadcaster::broadcastAttackStart( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    const int idCharacter = payload[ "idCharacter" ].asInt();

    Engine::CharacterEventAttackStartDTO message;
    message.setIdCharacter( idCharacter );
    message.setX( payload[ "x" ].asInt() );
    message.setY( payload[ "y" ].asInt() );
    message.setZ( payload[ "z" ].asInt() );
    message.setCastSeconds( payload[ "castSeconds" ].asDouble() );

    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    std::vector<int> receivers = worldRuntime.charactersNear( idCharacter );
    receivers.push_back( idCharacter );

    broadcast( receivers, Engine::JsonHelper::writeJsonString( message.toJson() ) );
}

void EntityBroadcaster::broadcastAttack( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    const int idCharacter = payload[ "idCharacter" ].asInt();

    Engine::CharacterEventAttackDTO message;
    message.setIdCharacter( idCharacter );
    message.setX( payload[ "x" ].asInt() );
    message.setY( payload[ "y" ].asInt() );
    message.setZ( payload[ "z" ].asInt() );

    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    std::vector<int> receivers = worldRuntime.charactersNear( idCharacter );
    receivers.push_back( idCharacter );

    broadcast( receivers, Engine::JsonHelper::writeJsonString( message.toJson() ) );
}

void EntityBroadcaster::broadcastCreature( const Json::Value& payload ) {
    const Engine::CreatureDTO message = Engine::CreatureDTO::fromJson( payload );

    broadcastNear( payload, Engine::JsonHelper::writeJsonString( message.toJson() ) );
}

void EntityBroadcaster::broadcastNear( const Json::Value& payload, const std::string& message ) {
    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    broadcast( worldRuntime.charactersNear( positionFromPayload( payload ) ), message );
}

Engine::EntityPositionModel EntityBroadcaster::positionFromPayload( const Json::Value& payload ) {
    Engine::EntityPositionModel position;
    position.setX( payload[ "x" ].asInt() );
    position.setY( payload[ "y" ].asInt() );
    position.setZ( payload[ "z" ].asInt() );

    return position;
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

} // namespace Server
