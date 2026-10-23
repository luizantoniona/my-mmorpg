#include "ClientMessageReceiver.h"

#include <cstdlib>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/Character/CharacterIntentActionDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterIntentDropDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterIntentEquipDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterIntentMoveDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterIntentPickupDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterIntentUnequipDTO.h>
#include <MMORPGEngine/Entity/CombatActionHelper.h>
#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeHelper.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>
#include <MMORPGServer/Server/Runtime/World/Command/Character/CharacterAttackEntityCommand.h>
#include <MMORPGServer/Server/Runtime/World/Command/Character/CharacterDropItemCommand.h>
#include <MMORPGServer/Server/Runtime/World/Command/Character/CharacterEquipItemCommand.h>
#include <MMORPGServer/Server/Runtime/World/Command/Character/CharacterMoveCommand.h>
#include <MMORPGServer/Server/Runtime/World/Command/Character/CharacterPickupItemCommand.h>
#include <MMORPGServer/Server/Runtime/World/Command/Character/CharacterUnequipItemCommand.h>

namespace Server {

ClientMessageReceiver::ClientMessageReceiver() {
    _worldRuntime = &Engine::Singleton<WorldManager>::instance().runtime();
}

void ClientMessageReceiver::receive( int idCharacter, const std::string& message ) {
    if ( message.empty() ) {
        return;
    }

    Json::Value messageJson = Engine::JsonHelper::parseJsonString( message );
    if ( messageJson.isNull() || !messageJson.isObject() ) {
        return;
    }

    const Engine::ClientMessageType type = Engine::ClientMessageTypeHelper::fromMessage( messageJson );

    switch ( type ) {
    case Engine::ClientMessageType::CHARACTER_INTENT_MOVE:
        receiveMove( idCharacter, messageJson );
        break;
    case Engine::ClientMessageType::CHARACTER_INTENT_ACTION:
        receiveAction( idCharacter, messageJson );
        break;
    case Engine::ClientMessageType::CHARACTER_INTENT_EQUIP:
        receiveEquip( idCharacter, messageJson );
        break;
    case Engine::ClientMessageType::CHARACTER_INTENT_UNEQUIP:
        receiveUnequip( idCharacter, messageJson );
        break;
    case Engine::ClientMessageType::CHARACTER_INTENT_DROP:
        receiveDrop( idCharacter, messageJson );
        break;
    case Engine::ClientMessageType::CHARACTER_INTENT_PICKUP:
        receivePickup( idCharacter, messageJson );
        break;
    default:
        break;
    }
}

void ClientMessageReceiver::receiveMove( int idCharacter, const Json::Value& messageJson ) {
    const Engine::CharacterIntentMoveDTO input = Engine::CharacterIntentMoveDTO::fromJson( messageJson );
    const int dx = input.dx();
    const int dy = input.dy();

    if ( std::abs( dx ) > 1 || std::abs( dy ) > 1 ) {
        qWarning() << "[ClientMessageReceiver] Rejected move: out-of-range step [CHARACTER]" << idCharacter << "[DX]" << dx << "[DY]" << dy;
        return;
    }

    _worldRuntime->enqueueCommand( std::make_unique<CharacterMoveCommand>( idCharacter, dx, dy ) );
}

void ClientMessageReceiver::receiveAction( int idCharacter, const Json::Value& messageJson ) {
    const Engine::CharacterIntentActionDTO input = Engine::CharacterIntentActionDTO::fromJson( messageJson );

    if ( !input.action() ) {
        qWarning() << "[ClientMessageReceiver] Rejected action: unknown action [CHARACTER]" << idCharacter;
        return;
    }

    switch ( *input.action() ) {
    case Engine::CombatActionEnum::ATTACK:
        receiveAttack( idCharacter, input );
        break;
    case Engine::CombatActionEnum::DODGE:
    case Engine::CombatActionEnum::BLOCK:
        // TODO: Esquiva e Bloqueio ainda não existem no WorldCombatSystem; por ora só registra a intenção
        qInfo() << "[ClientMessageReceiver] Action intent ignored [CHARACTER]" << idCharacter << "[ACTION]" << Engine::CombatActionHelper::toString( *input.action() ).c_str();
        break;
    }
}

void ClientMessageReceiver::receiveAttack( int idCharacter, const Engine::CharacterIntentActionDTO& input ) {
    const int dx = input.dx();
    const int dy = input.dy();

    if ( std::abs( dx ) > 1 || std::abs( dy ) > 1 || ( dx == 0 && dy == 0 ) ) {
        qWarning() << "[ClientMessageReceiver] Rejected attack: invalid direction [CHARACTER]" << idCharacter << "[DX]" << dx << "[DY]" << dy;
        return;
    }

    _worldRuntime->enqueueCommand( std::make_unique<CharacterAttackEntityCommand>( idCharacter, dx, dy ) );
}

void ClientMessageReceiver::receiveEquip( int idCharacter, const Json::Value& messageJson ) {
    const Engine::CharacterIntentEquipDTO input = Engine::CharacterIntentEquipDTO::fromJson( messageJson );

    if ( !input.position() ) {
        qWarning() << "[ClientMessageReceiver] Rejected equip: invalid position [CHARACTER]" << idCharacter;
        return;
    }

    _worldRuntime->enqueueCommand( std::make_unique<CharacterEquipItemCommand>( idCharacter, *input.position() ) );
}

void ClientMessageReceiver::receiveUnequip( int idCharacter, const Json::Value& messageJson ) {
    const Engine::CharacterIntentUnequipDTO input = Engine::CharacterIntentUnequipDTO::fromJson( messageJson );

    if ( !input.slot() || *input.slot() == Engine::EquipmentSlotEnum::NONE ) {
        qWarning() << "[ClientMessageReceiver] Rejected unequip: invalid slot [CHARACTER]" << idCharacter;
        return;
    }

    _worldRuntime->enqueueCommand( std::make_unique<CharacterUnequipItemCommand>( idCharacter, *input.slot() ) );
}

void ClientMessageReceiver::receiveDrop( int idCharacter, const Json::Value& messageJson ) {
    const Engine::CharacterIntentDropDTO input = Engine::CharacterIntentDropDTO::fromJson( messageJson );

    if ( !input.position() ) {
        qWarning() << "[ClientMessageReceiver] Rejected drop: invalid position [CHARACTER]" << idCharacter;
        return;
    }

    _worldRuntime->enqueueCommand( std::make_unique<CharacterDropItemCommand>( idCharacter, *input.position() ) );
}

void ClientMessageReceiver::receivePickup( int idCharacter, const Json::Value& messageJson ) {
    const Engine::CharacterIntentPickupDTO input = Engine::CharacterIntentPickupDTO::fromJson( messageJson );

    if ( !input.idGroundItem() ) {
        qWarning() << "[ClientMessageReceiver] Rejected pickup: invalid ground item [CHARACTER]" << idCharacter;
        return;
    }

    _worldRuntime->enqueueCommand( std::make_unique<CharacterPickupItemCommand>( idCharacter, *input.idGroundItem() ) );
}

} // namespace Server
