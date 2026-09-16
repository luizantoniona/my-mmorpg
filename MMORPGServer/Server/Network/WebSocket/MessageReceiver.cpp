#include "MessageReceiver.h"

#include <cstdlib>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/EntityOrientationModel.h>
#include <MMORPGEngine/Entity/EntityStateDTO.h>
#include <MMORPGEngine/World/WorldModel.h>

namespace Server {

MessageReceiver::MessageReceiver() {
    _worldManager = &Engine::Singleton<WorldManager>::instance();
}

void MessageReceiver::receive( const drogon::WebSocketConnectionPtr& connection, int idCharacter, const std::string& message ) {
    if ( message.empty() ) {
        return;
    }

    Json::Value messageJson = Engine::JsonHelper::parseJsonString( message );
    if ( messageJson.isNull() || !messageJson.isObject() ) {
        return;
    }

    const std::string type = messageJson.get( "type", "" ).asString();

    if ( type == "move" ) {
        receiveMove( connection, idCharacter, messageJson );
    }
}

void MessageReceiver::receiveMove( const drogon::WebSocketConnectionPtr& connection, int idCharacter, const Json::Value& messageJson ) {
    Engine::CharacterModel* character = _worldManager->character( idCharacter );
    if ( !character ) {
        return;
    }

    const int dx = messageJson.get( "dx", 0 ).asInt();
    const int dy = messageJson.get( "dy", 0 ).asInt();

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

    const Engine::WorldModel* world = _worldManager->world();
    const Engine::WorldTileModel* worldTile = world ? world->tile( newX, newY, z ) : nullptr;

    if ( worldTile && worldTile->tileModel() && worldTile->tileModel()->isWalkable() ) {
        _worldManager->moveCharacter( idCharacter, newX, newY, z );

        qInfo() << "[MessageReceiver] Character moved [CHARACTER]" << idCharacter << "[X]" << newX << "[Y]" << newY << "[Z]" << z;

    } else {
        qInfo() << "[MessageReceiver] Move blocked [CHARACTER]" << idCharacter << "[X]" << newX << "[Y]" << newY << "[Z]" << z;
    }

    const Engine::EntityPositionModel finalPosition = character->position();

    Engine::EntityStateDTO state;
    state.setIdCharacter( idCharacter );
    state.setX( finalPosition.x() );
    state.setY( finalPosition.y() );
    state.setZ( finalPosition.z() );
    state.setOrientation( character->orientation().direction() );
    state.setWorldName( world ? world->name().toStdString() : "" );

    connection->send( Engine::JsonHelper::writeJsonString( state.toJson() ) );
}

} // namespace Server
