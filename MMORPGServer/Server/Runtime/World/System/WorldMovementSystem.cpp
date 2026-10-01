#include "WorldMovementSystem.h"

#include <QDebug>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Event/WorldEvent.h>
#include <MMORPGServer/Server/Event/WorldEventType.h>
#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace Server {

WorldMovementSystem::WorldMovementSystem( WorldRuntime& runtime ) :
    WorldSystem( runtime ) {
}

void WorldMovementSystem::onTick() {
}

bool WorldMovementSystem::move( int idCharacter, int dx, int dy ) {
    Engine::CharacterModel* character = _runtime.character( idCharacter );
    if ( !character ) {
        return false;
    }

    const Engine::EntityPositionModel currentPosition = character->position();
    const int newX = currentPosition.x() + dx;
    const int newY = currentPosition.y() + dy;
    const int z = currentPosition.z();

    const Engine::WorldModel* world = _runtime.world();
    const Engine::WorldTileModel* worldTile = world ? world->tile( newX, newY, z ) : nullptr;

    if ( !worldTile || !worldTile->tileModel() || !worldTile->tileModel()->isWalkable() || _runtime.isPositionOccupied( newX, newY, z ) || !character->movement().isReady( _runtime.tickRate() ) ) {
        qInfo() << "[WorldMovementSystem] Move blocked [CHARACTER]" << idCharacter << "[X]" << newX << "[Y]" << newY << "[Z]" << z;
        return false;
    }

    moveCharacter( idCharacter, newX, newY, z );

    qInfo() << "[WorldMovementSystem] Character moved [CHARACTER]" << idCharacter << "[X]" << newX << "[Y]" << newY << "[Z]" << z;

    return true;
}

void WorldMovementSystem::moveCharacter( int idCharacter, int x, int y, int z ) {
    _runtime.combatSystem().cancelCharacterAttack( idCharacter );

    {
        std::lock_guard<std::mutex> lock( _runtime.mutex() );

        Engine::CharacterModel* characterPtr = _runtime.characterLocked( idCharacter );
        if ( !characterPtr ) {
            return;
        }

        const Engine::EntityPositionModel previousPosition = characterPtr->position();

        characterPtr->position().setX( x );
        characterPtr->position().setY( y );
        characterPtr->position().setZ( z );

        characterPtr->movement().setCounter( 0 );

        _runtime.spatialIndex().updateCharacterChunk( characterPtr, previousPosition );
    }

    Json::Value payload;
    payload[ "idCharacter" ] = idCharacter;
    payload[ "x" ] = x;
    payload[ "y" ] = y;
    payload[ "z" ] = z;

    _runtime.eventBus().publish( WorldEvent( WorldEventType::CHARACTER_MOVED, payload ) );
}

} // namespace Server
