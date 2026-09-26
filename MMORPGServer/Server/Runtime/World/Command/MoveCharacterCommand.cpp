#include "MoveCharacterCommand.h"

#include <utility>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/Character/OwnCharacterDTO.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace Server {

MoveCharacterCommand::MoveCharacterCommand( int idCharacter, int dx, int dy, std::function<void( const std::string& )> respond ) :
    _respond( std::move( respond ) ),
    _idCharacter( idCharacter ),
    _dx( dx ),
    _dy( dy ) {
}

void MoveCharacterCommand::execute( WorldRuntime& runtime ) {
    Engine::CharacterModel* character = runtime.character( _idCharacter );
    if ( !character ) {
        return;
    }

    const Engine::EntityPositionModel currentPosition = character->position();
    const int newX = currentPosition.x() + _dx;
    const int newY = currentPosition.y() + _dy;
    const int z = currentPosition.z();

    const Engine::WorldModel* world = runtime.world();
    const Engine::WorldTileModel* worldTile = world ? world->tile( newX, newY, z ) : nullptr;

    if ( worldTile && worldTile->tileModel() && worldTile->tileModel()->isWalkable() && !runtime.isPositionOccupied( newX, newY, z ) && character->movement().isReady( runtime.tickRate() ) ) {
        runtime.moveCharacter( _idCharacter, newX, newY, z );

        qInfo() << "[MoveCharacterCommand] Character moved [CHARACTER]" << _idCharacter << "[X]" << newX << "[Y]" << newY << "[Z]" << z;

    } else {
        qInfo() << "[MoveCharacterCommand] Move blocked [CHARACTER]" << _idCharacter << "[X]" << newX << "[Y]" << newY << "[Z]" << z;
    }

    _respond( Engine::JsonHelper::writeJsonString( Engine::OwnCharacterDTO::fromModel( character ).toJson() ) );
}

} // namespace Server
