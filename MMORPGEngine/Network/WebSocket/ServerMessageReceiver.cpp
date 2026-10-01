#include "ServerMessageReceiver.h"

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Entity/Character/CharacterDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterEventAttackDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterEventAttackStartDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterEventLeaveDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureEventAttackDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureEventAttackStartDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureEventLeaveDTO.h>
#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

ServerMessageReceiver::ServerMessageReceiver( QObject* parent ) :
    QObject( parent ) {
}

ServerMessageReceiver::~ServerMessageReceiver() = default;

void ServerMessageReceiver::receiveMessage( const QString& message ) {
    Json::Value json = JsonHelper::parseJsonString( message.toStdString() );

    if ( json.isNull() || !json.isObject() ) {
        emit errorReceived( tr( "Invalid server response" ) );
        return;
    }

    if ( json.isMember( "error" ) ) {
        emit errorReceived( QString::fromStdString( json[ "error" ].asString() ) );
        return;
    }

    const ServerMessageType type = ServerMessageTypeHelper::fromMessage( json );

    switch ( type ) {
    case ServerMessageType::OWN_CHARACTER: {
        const OwnCharacterDTO state = OwnCharacterDTO::fromJson( json );
        emit ownCharacterReceived( state );
        return;
    }

    case ServerMessageType::OWN_EQUIPMENT: {
        const OwnEquipmentDTO state = OwnEquipmentDTO::fromJson( json );
        emit ownEquipmentReceived( state );
        return;
    }

    case ServerMessageType::OWN_INVENTORY: {
        const OwnInventoryDTO state = OwnInventoryDTO::fromJson( json );
        emit ownInventoryReceived( state );
        return;
    }

    case ServerMessageType::CHARACTER: {
        const CharacterDTO state = CharacterDTO::fromJson( json );
        emit characterStateReceived( state.idCharacter(), state.x(), state.y(), state.z(), state.movementCooldownSeconds() );
        return;
    }

    case ServerMessageType::CHARACTER_EVENT_ATTACK_START: {
        const CharacterEventAttackStartDTO event = CharacterEventAttackStartDTO::fromJson( json );
        emit characterEventAttackStartReceived( event.idCharacter(), event.x(), event.y(), event.z(), event.castSeconds() );
        return;
    }

    case ServerMessageType::CHARACTER_EVENT_ATTACK: {
        const CharacterEventAttackDTO event = CharacterEventAttackDTO::fromJson( json );
        emit characterEventAttackReceived( event.idCharacter(), event.x(), event.y(), event.z() );
        return;
    }

    case ServerMessageType::CHARACTER_EVENT_LEAVE: {
        const CharacterEventLeaveDTO event = CharacterEventLeaveDTO::fromJson( json );
        emit characterEventLeaveReceived( event.idCharacter() );
        return;
    }

    case ServerMessageType::CREATURE: {
        const CreatureDTO state = CreatureDTO::fromJson( json );
        emit creatureStateReceived( state.idCreature(), state.x(), state.y(), state.z(), state.movementCooldownSeconds() );
        return;
    }

    case ServerMessageType::CREATURE_EVENT_ATTACK_START: {
        const CreatureEventAttackStartDTO event = CreatureEventAttackStartDTO::fromJson( json );
        emit creatureEventAttackStartReceived( event.idCreature(), event.x(), event.y(), event.z(), event.castSeconds() );
        return;
    }

    case ServerMessageType::CREATURE_EVENT_ATTACK: {
        const CreatureEventAttackDTO event = CreatureEventAttackDTO::fromJson( json );
        emit creatureEventAttackReceived( event.idCreature(), event.x(), event.y(), event.z() );
        return;
    }

    case ServerMessageType::CREATURE_EVENT_LEAVE: {
        const CreatureEventLeaveDTO event = CreatureEventLeaveDTO::fromJson( json );
        emit creatureEventLeaveReceived( event.idCreature() );
        return;
    }

    case ServerMessageType::WORLD_BASIC:
    case ServerMessageType::UNKNOWN:
        return;
    }
}

} // namespace Engine
