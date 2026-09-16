#include "EntityBroadcaster.h"

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/EntityOrientationEnum.h>
#include <MMORPGEngine/Entity/EntityStateDTO.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>
#include <MMORPGServer/Server/Network/WebSocket/CharacterConnectionRegistry.h>

namespace Server {

EntityBroadcaster::EntityBroadcaster() {
    auto& worldManager = Engine::Singleton<WorldManager>::instance();

    worldManager.eventBus().subscribe( WorldEventType::ENTITY_ENTERED, [ this ]( const WorldEvent& event ) {
        onEntityEntered( event );
    } );

    worldManager.eventBus().subscribe( WorldEventType::ENTITY_MOVED, [ this ]( const WorldEvent& event ) {
        onEntityMoved( event );
    } );

    worldManager.eventBus().subscribe( WorldEventType::ENTITY_LEFT, [ this ]( const WorldEvent& event ) {
        onEntityLeft( event );
    } );
}

void EntityBroadcaster::onEntityEntered( const WorldEvent& event ) {
    broadcastPosition( event );
}

void EntityBroadcaster::onEntityMoved( const WorldEvent& event ) {
    broadcastPosition( event );
}

void EntityBroadcaster::onEntityLeft( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    const int idCharacter = payload[ "idCharacter" ].asInt();

    Json::Value message;
    message[ "type" ] = "leave";
    message[ "idCharacter" ] = idCharacter;

    const std::string serialized = Engine::JsonHelper::writeJsonString( message );

    auto& connectionRegistry = Engine::Singleton<CharacterConnectionRegistry>::instance();

    for ( const Json::Value& nearbyIdCharacter : payload[ "nearby" ] ) {
        drogon::WebSocketConnectionPtr connection = connectionRegistry.connection( nearbyIdCharacter.asInt() );

        if ( connection ) {
            connection->send( serialized );
        }
    }
}

void EntityBroadcaster::broadcastPosition( const WorldEvent& event ) {
    const Json::Value& payload = event.payload();

    const int idCharacter = payload[ "idCharacter" ].asInt();
    const int x = payload[ "x" ].asInt();
    const int y = payload[ "y" ].asInt();
    const int z = payload[ "z" ].asInt();

    auto& worldManager = Engine::Singleton<WorldManager>::instance();
    const std::vector<int> nearbyCharacters = worldManager.charactersNear( idCharacter );

    if ( nearbyCharacters.empty() ) {
        return;
    }

    const Engine::WorldModel* world = worldManager.world();
    const Engine::CharacterModel* character = worldManager.character( idCharacter );

    Engine::EntityStateDTO state;
    state.setIdCharacter( idCharacter );
    state.setX( x );
    state.setY( y );
    state.setZ( z );
    state.setOrientation( character ? character->orientation().direction() : Engine::EntityOrientationEnum::SOUTH );
    state.setWorldName( world ? world->name().toStdString() : "" );

    const std::string message = Engine::JsonHelper::writeJsonString( state.toJson() );

    auto& connectionRegistry = Engine::Singleton<CharacterConnectionRegistry>::instance();

    for ( int nearbyIdCharacter : nearbyCharacters ) {
        drogon::WebSocketConnectionPtr connection = connectionRegistry.connection( nearbyIdCharacter );

        if ( connection ) {
            connection->send( message );
        }
    }
}

} // namespace Server
