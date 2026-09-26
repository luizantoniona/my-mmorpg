#include "MessageReceiver.h"

#include <cstdlib>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/Character/CharacterAttackDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterMoveDTO.h>
#include <MMORPGEngine/Entity/Character/OwnCharacterDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureModel.h>
#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeHelper.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>

namespace {

constexpr double ATTACK_DAMAGE = 5.0;
constexpr double ATTACK_STAMINA_COST = 10.0;

} // namespace

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
    Engine::CharacterModel* character = _worldRuntime->character( idCharacter );
    if ( !character ) {
        return;
    }

    const Engine::CharacterMoveDTO input = Engine::CharacterMoveDTO::fromJson( messageJson );
    const int dx = input.dx();
    const int dy = input.dy();

    if ( std::abs( dx ) > 1 || std::abs( dy ) > 1 ) {
        qWarning() << "[MessageReceiver] Rejected move: out-of-range step [CHARACTER]" << idCharacter << "[DX]" << dx << "[DY]" << dy;
        return;
    }

    const Engine::EntityPositionModel currentPosition = character->position();
    const int newX = currentPosition.x() + dx;
    const int newY = currentPosition.y() + dy;
    const int z = currentPosition.z();

    const Engine::WorldModel* world = _worldRuntime->world();
    const Engine::WorldTileModel* worldTile = world ? world->tile( newX, newY, z ) : nullptr;

    if ( worldTile && worldTile->tileModel() && worldTile->tileModel()->isWalkable() && !_worldRuntime->isPositionOccupied( newX, newY, z ) && _worldRuntime->isCharacterMoveDue( idCharacter ) ) {
        _worldRuntime->moveCharacter( idCharacter, newX, newY, z );

        qInfo() << "[MessageReceiver] Character moved [CHARACTER]" << idCharacter << "[X]" << newX << "[Y]" << newY << "[Z]" << z;

    } else {
        qInfo() << "[MessageReceiver] Move blocked [CHARACTER]" << idCharacter << "[X]" << newX << "[Y]" << newY << "[Z]" << z;
    }

    connection->send( Engine::JsonHelper::writeJsonString( Engine::OwnCharacterDTO::fromModel( character ).toJson() ) );
}

void MessageReceiver::receiveAttack( const drogon::WebSocketConnectionPtr& connection, int idCharacter, const Json::Value& messageJson ) {
    Engine::CharacterModel* character = _worldRuntime->character( idCharacter );
    if ( !character ) {
        return;
    }

    const Engine::CharacterAttackDTO input = Engine::CharacterAttackDTO::fromJson( messageJson );
    const int dx = input.dx();
    const int dy = input.dy();

    if ( std::abs( dx ) > 1 || std::abs( dy ) > 1 || ( dx == 0 && dy == 0 ) ) {
        qWarning() << "[MessageReceiver] Rejected attack: invalid direction [CHARACTER]" << idCharacter << "[DX]" << dx << "[DY]" << dy;
        return;
    }

    const Engine::EntityPositionModel currentPosition = character->position();
    const int targetX = currentPosition.x() + dx;
    const int targetY = currentPosition.y() + dy;
    const int z = currentPosition.z();

    Engine::CreatureModel* creature = _worldRuntime->creatureAt( targetX, targetY, z );

    if ( creature && character->vitals().stamina() >= ATTACK_STAMINA_COST && _worldRuntime->isCharacterAttackDue( idCharacter ) ) {
        _worldRuntime->attackCreature( idCharacter, creature->idCreature(), ATTACK_DAMAGE, ATTACK_STAMINA_COST );

        qInfo() << "[MessageReceiver] Character attacked creature [CHARACTER]" << idCharacter << "[CREATURE]" << creature->idCreature();

    } else {
        qInfo() << "[MessageReceiver] Attack blocked [CHARACTER]" << idCharacter << "[X]" << targetX << "[Y]" << targetY << "[Z]" << z;
    }
}

} // namespace Server
