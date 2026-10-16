#include "EntityBroadcaster.h"

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/Character/CharacterDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterEventAttackDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterEventAttackStartDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterEventLeaveDTO.h>
#include <MMORPGEngine/Entity/Character/OwnCharacterDTO.h>
#include <MMORPGEngine/Entity/Character/OwnCombatDTO.h>
#include <MMORPGEngine/Entity/Character/OwnEquipmentDTO.h>
#include <MMORPGEngine/Entity/Character/OwnInventoryDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureEventAttackDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureEventAttackStartDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureEventLeaveDTO.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/Entity/GroundItem/GroundItemDTO.h>
#include <MMORPGEngine/Entity/GroundItem/GroundItemEventLeaveDTO.h>
#include <MMORPGEngine/World/Chunk/ChunkDTO.h>
#include <MMORPGEngine/World/Chunk/ChunkEventLeaveDTO.h>
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

    worldRuntime.eventBus().subscribe( WorldEventType::CHARACTER_VISIBILITY_CHANGED, [ this ]( const WorldEvent& event ) {
        onCharacterVisibilityChanged( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::CREATURE_VISIBILITY_CHANGED, [ this ]( const WorldEvent& event ) {
        onCreatureVisibilityChanged( event );
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

    worldRuntime.eventBus().subscribe( WorldEventType::CHARACTER_EQUIPMENT_CHANGED, [ this ]( const WorldEvent& event ) {
        onCharacterEquipmentChanged( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::CHARACTER_INVENTORY_CHANGED, [ this ]( const WorldEvent& event ) {
        onCharacterInventoryChanged( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::GROUND_ITEM_ENTERED, [ this ]( const WorldEvent& event ) {
        onGroundItemEntered( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::GROUND_ITEM_LEFT, [ this ]( const WorldEvent& event ) {
        onGroundItemLeft( event );
    } );

    worldRuntime.eventBus().subscribe( WorldEventType::CREATURE_ENTERED, [ this ]( const WorldEvent& event ) {
        onCreatureEntered( event );
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
    sendTerrain( event );
    sendOwnCharacter( event );
    sendOwnCombat( event );
    sendOwnEquipment( event );
    sendOwnInventory( event );
    sendNearbyCharacters( event );
    sendCreatures( event );
    sendGroundItems( event );
    broadcastCharacter( event );
}

void EntityBroadcaster::onCharacterMoved( const WorldEvent& event ) {
    broadcastCharacter( event );
}

void EntityBroadcaster::onCharacterVisibilityChanged( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();
    const int idCharacter = payload[ "idCharacter" ].asInt();

    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    const WorldVisibilityDelta delta = worldRuntime.visibilityDelta( previousPositionFromPayload( payload ), positionFromPayload( payload ), idCharacter );

    for ( const ChunkCoordinate& chunk : delta.terrainChunksLeft ) {
        Engine::ChunkEventLeaveDTO message;
        message.setChunkX( chunk.x );
        message.setChunkY( chunk.y );
        sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( message.toJson() ) );
    }

    const Engine::WorldModel* world = worldRuntime.world();

    for ( const ChunkCoordinate& chunk : delta.terrainChunksEntered ) {
        if ( world ) {
            sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( Engine::ChunkDTO::fromModel( *world, chunk.x, chunk.y ).toJson() ) );
        }
    }

    for ( int idLeft : delta.charactersLeft ) {
        Engine::CharacterEventLeaveDTO fromMover;
        fromMover.setIdCharacter( idLeft );
        sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( fromMover.toJson() ) );

        Engine::CharacterEventLeaveDTO fromOther;
        fromOther.setIdCharacter( idCharacter );
        sendToCharacter( idLeft, Engine::JsonHelper::writeJsonString( fromOther.toJson() ) );
    }

    for ( int idEntered : delta.charactersEntered ) {
        const Engine::CharacterModel* entered = worldRuntime.character( idEntered );
        if ( entered ) {
            sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( Engine::CharacterDTO::fromModel( entered ).toJson() ) );
        }
    }

    for ( int idLeft : delta.creaturesLeft ) {
        Engine::CreatureEventLeaveDTO message;
        message.setIdCreature( idLeft );
        sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( message.toJson() ) );
    }

    for ( int idEntered : delta.creaturesEntered ) {
        const Engine::CreatureModel* entered = worldRuntime.creature( idEntered );
        if ( entered ) {
            sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( Engine::CreatureDTO::fromModel( entered ).toJson() ) );
        }
    }

    for ( int idLeft : delta.groundItemsLeft ) {
        Engine::GroundItemEventLeaveDTO message;
        message.setIdGroundItem( idLeft );
        sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( message.toJson() ) );
    }

    for ( int idEntered : delta.groundItemsEntered ) {
        const std::optional<Engine::GroundItemModel> entered = worldRuntime.groundItem( idEntered );
        if ( entered ) {
            sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( Engine::GroundItemDTO::fromModel( &*entered ).toJson() ) );
        }
    }
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

void EntityBroadcaster::onCharacterEquipmentChanged( const WorldEvent& event ) {
    sendOwnEquipment( event );
    sendOwnInventory( event );
    sendOwnCombat( event );
}

void EntityBroadcaster::onCharacterInventoryChanged( const WorldEvent& event ) {
    sendOwnInventory( event );
}

void EntityBroadcaster::onGroundItemEntered( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    broadcastNear( payload, Engine::JsonHelper::writeJsonString( Engine::GroundItemDTO::fromJson( payload ).toJson() ) );
}

void EntityBroadcaster::onGroundItemLeft( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    Engine::GroundItemEventLeaveDTO message;
    message.setIdGroundItem( payload[ "idGroundItem" ].asInt() );

    broadcastNear( payload, Engine::JsonHelper::writeJsonString( message.toJson() ) );
}

void EntityBroadcaster::onCreatureEntered( const WorldEvent& event ) {
    broadcastCreature( event.payload() );
}

void EntityBroadcaster::onCreatureMoved( const WorldEvent& event ) {
    broadcastCreature( event.payload() );
}

void EntityBroadcaster::onCreatureVisibilityChanged( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    const WorldVisibilityDelta delta = worldRuntime.visibilityDelta( previousPositionFromPayload( payload ), positionFromPayload( payload ), -1 );

    Engine::CreatureEventLeaveDTO message;
    message.setIdCreature( payload[ "idCreature" ].asInt() );
    const std::string messageJson = Engine::JsonHelper::writeJsonString( message.toJson() );

    for ( int idLeft : delta.charactersLeft ) {
        sendToCharacter( idLeft, messageJson );
    }
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

void EntityBroadcaster::sendTerrain( const WorldEvent& event ) {
    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    const Engine::WorldModel* world = worldRuntime.world();
    if ( !world ) {
        return;
    }

    const int idCharacter = event.payload()[ "idCharacter" ].asInt();

    for ( const ChunkCoordinate& chunk : worldRuntime.terrainChunksNear( positionFromPayload( event.payload() ) ) ) {
        sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( Engine::ChunkDTO::fromModel( *world, chunk.x, chunk.y ).toJson() ) );
    }
}

void EntityBroadcaster::sendOwnCharacter( const WorldEvent& event ) {
    const int idCharacter = event.payload()[ "idCharacter" ].asInt();

    const Engine::CharacterModel* character = Engine::Singleton<WorldManager>::instance().runtime().character( idCharacter );
    if ( !character ) {
        return;
    }

    sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( Engine::OwnCharacterDTO::fromModel( character ).toJson() ) );
}

void EntityBroadcaster::sendOwnCombat( const WorldEvent& event ) {
    const int idCharacter = event.payload()[ "idCharacter" ].asInt();

    const Engine::CharacterModel* character = Engine::Singleton<WorldManager>::instance().runtime().character( idCharacter );
    if ( !character ) {
        return;
    }

    sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( Engine::OwnCombatDTO::fromModel( character ).toJson() ) );
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

void EntityBroadcaster::sendGroundItems( const WorldEvent& event ) {
    const int idCharacter = event.payload()[ "idCharacter" ].asInt();

    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    for ( const Engine::GroundItemModel& groundItem : worldRuntime.groundItemsNear( positionFromPayload( event.payload() ) ) ) {
        sendToCharacter( idCharacter, Engine::JsonHelper::writeJsonString( Engine::GroundItemDTO::fromModel( &groundItem ).toJson() ) );
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

Engine::EntityPositionModel EntityBroadcaster::previousPositionFromPayload( const Json::Value& payload ) {
    Engine::EntityPositionModel position;
    position.setX( payload[ "previousX" ].asInt() );
    position.setY( payload[ "previousY" ].asInt() );
    position.setZ( payload[ "previousZ" ].asInt() );

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
