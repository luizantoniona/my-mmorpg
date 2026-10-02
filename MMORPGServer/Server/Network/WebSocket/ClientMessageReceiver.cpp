#include "ClientMessageReceiver.h"

#include <cstdlib>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/Character/CharacterIntentActionDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterIntentMoveDTO.h>
#include <MMORPGEngine/Entity/CombatActionHelper.h>
#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeHelper.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>
#include <MMORPGServer/Server/Runtime/World/Command/AttackCharacterCommand.h>
#include <MMORPGServer/Server/Runtime/World/Command/MoveCharacterCommand.h>

namespace Server {

ClientMessageReceiver::ClientMessageReceiver() {
    _worldRuntime = &Engine::Singleton<WorldManager>::instance().runtime();
}

void ClientMessageReceiver::receive( const drogon::WebSocketConnectionPtr& connection, int idCharacter, const std::string& message ) {
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
        receiveMove( connection, idCharacter, messageJson );
        break;
    case Engine::ClientMessageType::CHARACTER_INTENT_ACTION:
        receiveAction( idCharacter, messageJson );
        break;
    default:
        break;
    }
}

void ClientMessageReceiver::receiveMove( const drogon::WebSocketConnectionPtr& connection, int idCharacter, const Json::Value& messageJson ) {
    const Engine::CharacterIntentMoveDTO input = Engine::CharacterIntentMoveDTO::fromJson( messageJson );
    const int dx = input.dx();
    const int dy = input.dy();

    if ( std::abs( dx ) > 1 || std::abs( dy ) > 1 ) {
        qWarning() << "[ClientMessageReceiver] Rejected move: out-of-range step [CHARACTER]" << idCharacter << "[DX]" << dx << "[DY]" << dy;
        return;
    }

    _worldRuntime->enqueueCommand( std::make_unique<MoveCharacterCommand>( idCharacter, dx, dy, [ connection ]( const std::string& json ) {
        connection->send( json );
    } ) );
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

    _worldRuntime->enqueueCommand( std::make_unique<AttackCharacterCommand>( idCharacter, dx, dy ) );
}

} // namespace Server
