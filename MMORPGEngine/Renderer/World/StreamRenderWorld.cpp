#include "StreamRenderWorld.h"

#include <MMORPGEngine/Renderer/EntityTextureModel.h>

namespace Engine {

StreamRenderWorld::StreamRenderWorld( QObject* parent ) :
    RenderWorld( parent ) {
}

QList<RenderWorld::Entity> StreamRenderWorld::entities( int z ) const {
    QList<Entity> result;

    appendEntities( result, _ownCharacter, z, EntityTextureModel::characterTexture() );
    appendEntities( result, _characters, z, EntityTextureModel::characterTexture() );
    appendEntities( result, _creatures, z, EntityTextureModel::creatureTexture() );

    return result;
}

void StreamRenderWorld::setOwnCharacter( int idCharacter, int x, int y, int z, double movementSeconds ) {
    if ( !_ownCharacter.isEmpty() && !_ownCharacter.contains( idCharacter ) ) {
        _ownCharacter.clear();
    }

    updateEntity( _ownCharacter, idCharacter, x, y, z, movementSeconds );
}

void StreamRenderWorld::addCharacter( int idCharacter, int x, int y, int z, double movementSeconds ) {
    updateEntity( _characters, idCharacter, x, y, z, movementSeconds );
}

void StreamRenderWorld::removeCharacter( int idCharacter ) {
    _characters.remove( idCharacter );
}

void StreamRenderWorld::addCreature( int idCreature, int x, int y, int z, double movementSeconds ) {
    updateEntity( _creatures, idCreature, x, y, z, movementSeconds );
}

void StreamRenderWorld::removeCreature( int idCreature ) {
    _creatures.remove( idCreature );
}

void StreamRenderWorld::clearEntities() {
    _ownCharacter.clear();
    _characters.clear();
    _creatures.clear();
}

void StreamRenderWorld::updateEntity( QHash<int, EntityAnimationModel>& entities, int idEntity, int x, int y, int z, double movementSeconds ) {
    auto it = entities.find( idEntity );

    if ( it == entities.end() ) {
        entities.insert( idEntity, EntityAnimationModel( x, y, z, movementSeconds ) );
        return;
    }

    it->moveTo( x, y, z, movementSeconds );
}

void StreamRenderWorld::appendEntities( QList<Entity>& result, const QHash<int, EntityAnimationModel>& entities, int z, const QImage& texture ) {
    for ( auto it = entities.constBegin(); it != entities.constEnd(); ++it ) {
        if ( it->z() != z ) {
            continue;
        }

        Entity entity( it.key(), it->x(), it->y(), texture );
        entity.offsetX = it->offsetX();
        entity.offsetY = it->offsetY();

        result.append( entity );
    }
}

} // namespace Engine
