#include "MessageReceiver.h"

#include <cstdlib>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/Character/MoveInputDTO.h>
#include <MMORPGEngine/Entity/Character/OwnCharacterDTO.h>
#include <MMORPGEngine/Entity/EntityOrientationModel.h>
#include <MMORPGEngine/Entity/EntityVitalsModel.h>
#include <MMORPGEngine/Network/WebSocket/NetworkMessageTypeHelper.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>

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

    const Engine::NetworkMessageType type = Engine::NetworkMessageTypeHelper::fromMessage( messageJson );

    switch ( type ) {
    case Engine::NetworkMessageType::MOVE:
        receiveMove( connection, idCharacter, messageJson );
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

    const Engine::MoveInputDTO input = Engine::MoveInputDTO::fromJson( messageJson );
    const int dx = input.dx();
    const int dy = input.dy();

    if ( std::abs( dx ) > 1 || std::abs( dy ) > 1 ) {
        qWarning() << "[MessageReceiver] Rejected move: out-of-range step [CHARACTER]" << idCharacter << "[DX]" << dx << "[DY]" << dy;
        return;
    }

    if ( dx != 0 || dy != 0 ) {
        Engine::EntityOrientationModel orientation = character->orientation();
        orientation.setDirection( Engine::EntityOrientationModel::fromMovement( dx, dy ) );
        character->setOrientation( orientation );
    }

    const Engine::EntityPositionModel currentPosition = character->position();
    const int newX = currentPosition.x() + dx;
    const int newY = currentPosition.y() + dy;
    const int z = currentPosition.z();

    const Engine::WorldModel* world = _worldRuntime->world();
    const Engine::WorldTileModel* worldTile = world ? world->tile( newX, newY, z ) : nullptr;

    if ( worldTile && worldTile->tileModel() && worldTile->tileModel()->isWalkable() ) {
        _worldRuntime->moveCharacter( idCharacter, newX, newY, z );

        qInfo() << "[MessageReceiver] Character moved [CHARACTER]" << idCharacter << "[X]" << newX << "[Y]" << newY << "[Z]" << z;

    } else {
        qInfo() << "[MessageReceiver] Move blocked [CHARACTER]" << idCharacter << "[X]" << newX << "[Y]" << newY << "[Z]" << z;
    }

    const Engine::EntityPositionModel finalPosition = character->position();
    const Engine::EntityVitalsModel& vitals = character->vitals();

    Engine::OwnCharacterDTO state;
    state.setIdCharacter( idCharacter );
    state.setX( finalPosition.x() );
    state.setY( finalPosition.y() );
    state.setZ( finalPosition.z() );
    state.setOrientation( character->orientation().direction() );
    state.setHealth( vitals.health() );
    state.setMaxHealth( vitals.maxHealth() );
    state.setMana( vitals.mana() );
    state.setMaxMana( vitals.maxMana() );
    state.setStamina( vitals.stamina() );
    state.setMaxStamina( vitals.maxStamina() );

    connection->send( Engine::JsonHelper::writeJsonString( state.toJson() ) );
}

} // namespace Server
