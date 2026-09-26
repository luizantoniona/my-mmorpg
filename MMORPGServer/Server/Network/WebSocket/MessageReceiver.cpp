#include "MessageReceiver.h"

#include <cstdlib>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/Character/CharacterAttackDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterMoveDTO.h>
#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeHelper.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>
#include <MMORPGServer/Server/Runtime/World/Command/AttackCharacterCommand.h>
#include <MMORPGServer/Server/Runtime/World/Command/MoveCharacterCommand.h>

namespace Server {

MessageReceiver::MessageReceiver() {
    _worldRuntime = &Engine::Singleton<WorldManager>::instance().runtime();
}

void MessageReceiver::receive( const drogon::WebSocketConnectionPtr& connection, int idCharacter, const std::string& message ) {
    if ( message.empty() ) {
        return;
    }

    Json::Value messageJson = Engine::JsonHelper::parseJsonString( message );
    if ( messageJson.isNull() || !messageJson.isObject() ) {
        return;
    }

    const Engine::ClientMessageType type = Engine::ClientMessageTypeHelper::fromMessage( messageJson );

    switch ( type ) {
    case Engine::ClientMessageType::CHARACTER_MOVE:
        receiveMove( connection, idCharacter, messageJson );
        break;
    case Engine::ClientMessageType::CHARACTER_ATTACK:
        receiveAttack( connection, idCharacter, messageJson );
        break;
    default:
        break;
    }
}

void MessageReceiver::receiveMove( const drogon::WebSocketConnectionPtr& connection, int idCharacter, const Json::Value& messageJson ) {
    const Engine::CharacterMoveDTO input = Engine::CharacterMoveDTO::fromJson( messageJson );
    const int dx = input.dx();
    const int dy = input.dy();

    if ( std::abs( dx ) > 1 || std::abs( dy ) > 1 ) {
        qWarning() << "[MessageReceiver] Rejected move: out-of-range step [CHARACTER]" << idCharacter << "[DX]" << dx << "[DY]" << dy;
        return;
    }

    _worldRuntime->enqueueCommand( std::make_unique<MoveCharacterCommand>( idCharacter, dx, dy, [ connection ]( const std::string& json ) {
        connection->send( json );
    } ) );
}

void MessageReceiver::receiveAttack( const drogon::WebSocketConnectionPtr& connection, int idCharacter, const Json::Value& messageJson ) {
    const Engine::CharacterAttackDTO input = Engine::CharacterAttackDTO::fromJson( messageJson );
    const int dx = input.dx();
    const int dy = input.dy();

    if ( std::abs( dx ) > 1 || std::abs( dy ) > 1 || ( dx == 0 && dy == 0 ) ) {
        qWarning() << "[MessageReceiver] Rejected attack: invalid direction [CHARACTER]" << idCharacter << "[DX]" << dx << "[DY]" << dy;
        return;
    }

    _worldRuntime->enqueueCommand( std::make_unique<AttackCharacterCommand>( idCharacter, dx, dy ) );
}

} // namespace Server
