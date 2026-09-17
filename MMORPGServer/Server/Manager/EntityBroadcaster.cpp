#include "EntityBroadcaster.h"

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/Character/CharacterDTO.h>
#include <MMORPGEngine/Entity/Character/OwnCharacterDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureDTO.h>
#include <MMORPGEngine/Entity/EntityLeftDTO.h>
#include <MMORPGEngine/World/WorldBasicDTO.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>
#include <MMORPGServer/Server/Network/WebSocket/CharacterConnectionRegistry.h>

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
}

void EntityBroadcaster::onEntityEntered( const WorldEvent& event ) {
    sendWorldBasic( event );
    sendOwnCharacter( event );
    sendNearbyCharacters( event );
    sendCreatures( event );
    broadcastCharacter( event );
}

void EntityBroadcaster::onEntityMoved( const WorldEvent& event ) {
    broadcastCharacter( event );
}

void EntityBroadcaster::onEntityLeft( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    const int idCharacter = payload[ "idCharacter" ].asInt();

    Engine::EntityLeftDTO message;
    message.setIdCharacter( idCharacter );

    const std::string serialized = Engine::JsonHelper::writeJsonString( message.toJson() );

    auto& connectionRegistry = Engine::Singleton<CharacterConnectionRegistry>::instance();

    for ( const Json::Value& nearbyIdCharacter : payload[ "nearby" ] ) {
        drogon::WebSocketConnectionPtr connection = connectionRegistry.connection( nearbyIdCharacter.asInt() );

        if ( connection ) {
            connection->send( serialized );
        }
    }
}

void EntityBroadcaster::onEntityVitalsChanged( const WorldEvent& event ) {
    sendOwnCharacter( event );
    broadcastCharacter( event );
}

void EntityBroadcaster::sendWorldBasic( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    const int idCharacter = payload[ "idCharacter" ].asInt();

    drogon::WebSocketConnectionPtr connection = Engine::Singleton<CharacterConnectionRegistry>::instance().connection( idCharacter );
    if ( !connection ) {
        return;
    }

    const Engine::WorldModel* world = Engine::Singleton<WorldManager>::instance().runtime().world();
    if ( !world ) {
        return;
    }

    connection->send( Engine::JsonHelper::writeJsonString( Engine::WorldBasicDTO::fromModel( world ).toJson() ) );
}

void EntityBroadcaster::sendOwnCharacter( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    const int idCharacter = payload[ "idCharacter" ].asInt();

    drogon::WebSocketConnectionPtr connection = Engine::Singleton<CharacterConnectionRegistry>::instance().connection( idCharacter );
    if ( !connection ) {
        return;
    }

    const Engine::CharacterModel* character = Engine::Singleton<WorldManager>::instance().runtime().character( idCharacter );
    if ( !character ) {
        return;
    }

    connection->send( Engine::JsonHelper::writeJsonString( Engine::OwnCharacterDTO::fromModel( character ).toJson() ) );
}

void EntityBroadcaster::sendNearbyCharacters( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    const int idCharacter = payload[ "idCharacter" ].asInt();

    drogon::WebSocketConnectionPtr connection = Engine::Singleton<CharacterConnectionRegistry>::instance().connection( idCharacter );
    if ( !connection ) {
        return;
    }

    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    for ( int nearbyIdCharacter : worldRuntime.charactersNear( idCharacter ) ) {
        const Engine::CharacterModel* nearbyCharacter = worldRuntime.character( nearbyIdCharacter );
        if ( !nearbyCharacter ) {
            continue;
        }

        connection->send( Engine::JsonHelper::writeJsonString( Engine::CharacterDTO::fromModel( nearbyCharacter ).toJson() ) );
    }
}

void EntityBroadcaster::sendCreatures( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    const int idCharacter = payload[ "idCharacter" ].asInt();

    drogon::WebSocketConnectionPtr connection = Engine::Singleton<CharacterConnectionRegistry>::instance().connection( idCharacter );
    if ( !connection ) {
        return;
    }

    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();

    // TODO: Filter by proximity once creatures move/spawn dynamically (Backlog "Monstros")
    for ( const Engine::CreatureModel& creature : worldRuntime.creatures() ) {
        connection->send( Engine::JsonHelper::writeJsonString( Engine::CreatureDTO::fromModel( &creature ).toJson() ) );
    }
}

void EntityBroadcaster::broadcastCharacter( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    const int idCharacter = payload[ "idCharacter" ].asInt();

    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();
    const std::vector<int> nearbyCharacters = worldRuntime.charactersNear( idCharacter );

    if ( nearbyCharacters.empty() ) {
        return;
    }

    const Engine::CharacterModel* character = worldRuntime.character( idCharacter );
    if ( !character ) {
        return;
    }

    const std::string message = Engine::JsonHelper::writeJsonString( Engine::CharacterDTO::fromModel( character ).toJson() );

    auto& connectionRegistry = Engine::Singleton<CharacterConnectionRegistry>::instance();

    for ( int nearbyIdCharacter : nearbyCharacters ) {
        drogon::WebSocketConnectionPtr connection = connectionRegistry.connection( nearbyIdCharacter );

        if ( connection ) {
            connection->send( message );
        }
    }
}

} // namespace Server
