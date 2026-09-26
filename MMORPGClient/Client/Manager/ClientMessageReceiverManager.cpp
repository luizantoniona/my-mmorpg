#include "ClientMessageReceiverManager.h"

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Entity/Character/CharacterDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureDTO.h>
#include <MMORPGEngine/Entity/EntityLeftDTO.h>
#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

ClientMessageReceiverManager::ClientMessageReceiverManager( QObject* parent ) :
    QObject( parent ) {
}

ClientMessageReceiverManager::~ClientMessageReceiverManager() = default;

void ClientMessageReceiverManager::receiveMessage( const QString& message ) {
    Json::Value json = Engine::JsonHelper::parseJsonString( message.toStdString() );

    if ( json.isNull() || !json.isObject() ) {
        emit errorReceived( tr( "Invalid server response" ) );
        return;
    }

    if ( json.isMember( "error" ) ) {
        emit errorReceived( QString::fromStdString( json[ "error" ].asString() ) );
        return;
    }

    const Engine::ServerMessageType type = Engine::ServerMessageTypeHelper::fromMessage( json );

    switch ( type ) {
    case Engine::ServerMessageType::ENTITY_LEFT: {
        const Engine::EntityLeftDTO entityLeft = Engine::EntityLeftDTO::fromJson( json );
        emit entityLeftReceived( entityLeft.idCharacter() );
        return;
    }

    case Engine::ServerMessageType::OWN_CHARACTER: {
        const Engine::OwnCharacterDTO state = Engine::OwnCharacterDTO::fromJson( json );
        emit ownCharacterReceived( state );
        return;
    }

    case Engine::ServerMessageType::OWN_EQUIPMENT: {
        const Engine::OwnEquipmentDTO state = Engine::OwnEquipmentDTO::fromJson( json );
        emit ownEquipmentReceived( state );
        return;
    }

    case Engine::ServerMessageType::OWN_INVENTORY: {
        const Engine::OwnInventoryDTO state = Engine::OwnInventoryDTO::fromJson( json );
        emit ownInventoryReceived( state );
        return;
    }

    case Engine::ServerMessageType::CHARACTER: {
        const Engine::CharacterDTO state = Engine::CharacterDTO::fromJson( json );
        emit characterStateReceived( state.idCharacter(), state.x(), state.y(), state.z() );
        return;
    }

    case Engine::ServerMessageType::CREATURE: {
        const Engine::CreatureDTO state = Engine::CreatureDTO::fromJson( json );
        emit creatureStateReceived( state.idCreature(), state.x(), state.y(), state.z() );
        return;
    }

    case Engine::ServerMessageType::WORLD_BASIC:
    case Engine::ServerMessageType::UNKNOWN:
        return;
    }
}
