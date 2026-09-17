#include "EntityBroadcaster.h"

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/Character/CharacterDTO.h>
#include <MMORPGEngine/Entity/Character/OwnCharacterDTO.h>
#include <MMORPGEngine/Entity/EntityLeftDTO.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/Entity/EntityVitalsModel.h>
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

    const Engine::EntityPositionModel& position = character->position();
    const Engine::EntityVitalsModel& vitals = character->vitals();

    Engine::CharacterDTO state;
    state.setIdCharacter( idCharacter );
    state.setX( position.x() );
    state.setY( position.y() );
    state.setZ( position.z() );
    state.setHealth( vitals.health() );
    state.setMaxHealth( vitals.maxHealth() );
    state.setMana( vitals.mana() );
    state.setMaxMana( vitals.maxMana() );
    state.setStamina( vitals.stamina() );
    state.setMaxStamina( vitals.maxStamina() );

    const std::string message = Engine::JsonHelper::writeJsonString( state.toJson() );

    auto& connectionRegistry = Engine::Singleton<CharacterConnectionRegistry>::instance();

    for ( int nearbyIdCharacter : nearbyCharacters ) {
        drogon::WebSocketConnectionPtr connection = connectionRegistry.connection( nearbyIdCharacter );

        if ( connection ) {
            connection->send( message );
        }
    }
}

void EntityBroadcaster::sendOwnCharacter( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    const int idCharacter = payload[ "idCharacter" ].asInt();

    auto& worldRuntime = Engine::Singleton<WorldManager>::instance().runtime();
    const Engine::CharacterModel* character = worldRuntime.character( idCharacter );
    if ( !character ) {
        return;
    }

    drogon::WebSocketConnectionPtr connection = Engine::Singleton<CharacterConnectionRegistry>::instance().connection( idCharacter );
    if ( !connection ) {
        return;
    }

    const Engine::EntityPositionModel& position = character->position();
    const Engine::EntityVitalsModel& vitals = character->vitals();

    Engine::OwnCharacterDTO state;
    state.setIdCharacter( idCharacter );
    state.setX( position.x() );
    state.setY( position.y() );
    state.setZ( position.z() );
    state.setHealth( vitals.health() );
    state.setMaxHealth( vitals.maxHealth() );
    state.setMana( vitals.mana() );
    state.setMaxMana( vitals.maxMana() );
    state.setStamina( vitals.stamina() );
    state.setMaxStamina( vitals.maxStamina() );

    connection->send( Engine::JsonHelper::writeJsonString( state.toJson() ) );
}

} // namespace Server
