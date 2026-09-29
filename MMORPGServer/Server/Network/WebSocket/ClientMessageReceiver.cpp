#include "ClientMessageReceiver.h"

#include <cstdlib>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/Character/CharacterIntentAttackDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterIntentMoveDTO.h>
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
    case Engine::ClientMessageType::CHARACTER_INTENT_ATTACK:
        receiveAttack( connection, idCharacter, messageJson );
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

void ClientMessageReceiver::receiveAttack( const drogon::WebSocketConnectionPtr& connection, int idCharacter, const Json::Value& messageJson ) {
    const Engine::CharacterIntentAttackDTO input = Engine::CharacterIntentAttackDTO::fromJson( messageJson );
    const int dx = input.dx();
    const int dy = input.dy();

    if ( std::abs( dx ) > 1 || std::abs( dy ) > 1 || ( dx == 0 && dy == 0 ) ) {
        qWarning() << "[ClientMessageReceiver] Rejected attack: invalid direction [CHARACTER]" << idCharacter << "[DX]" << dx << "[DY]" << dy;
        return;
    }

    _worldRuntime->enqueueCommand( std::make_unique<AttackCharacterCommand>( idCharacter, dx, dy ) );
}

} // namespace Server
