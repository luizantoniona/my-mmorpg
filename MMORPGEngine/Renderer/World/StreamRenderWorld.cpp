#include "StreamRenderWorld.h"

#include <QDateTime>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Item/ItemCatalog.h>
#include <MMORPGEngine/Data/Item/ItemModel.h>
#include <MMORPGEngine/Renderer/EntityTextureModel.h>

namespace Engine {

StreamRenderWorld::StreamRenderWorld( QObject* parent ) :
    RenderWorld( parent ) {
}

QList<RenderWorld::Entity> StreamRenderWorld::entities( int z ) const {
    QList<Entity> result;

    appendEntities( result, _ownCharacter, z, EntityTextureModel::characterTexture() );
    appendEntities( result, _characters, z, EntityTextureModel::characterTexture() );

    for ( auto it = _creatures.constBegin(); it != _creatures.constEnd(); ++it ) {
        if ( it->z() != z ) {
            continue;
        }

        const int footprint = _creatureFootprints.value( it.key(), 1 );

        QImage texture = creatureTexture( it.key(), it.value() );
        if ( texture.isNull() ) {
            texture = EntityTextureModel::creatureTexture( footprint );
        }

        Entity entity( it.key(), it->x(), it->y(), texture );
        entity.offsetX = it->offsetX();
        entity.offsetY = it->offsetY();
        entity.footprint = footprint;

        result.append( entity );
    }

    const ItemCatalog& itemCatalog = Singleton<DataManager>::instance().itemCatalog();

    for ( auto it = _groundItems.constBegin(); it != _groundItems.constEnd(); ++it ) {
        if ( it->position().z() != z ) {
            continue;
        }

        const ItemModel* item = itemCatalog.item( it->idItem() );

        result.append( Entity( -it.key(), it->position().x(), it->position().y(), item ? item->texture() : QImage() ) );
    }

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

void StreamRenderWorld::addCreature( int idCreature, int x, int y, int z, double movementSeconds, int footprint, const QString& creatureType ) {
    updateEntity( _creatures, idCreature, x, y, z, movementSeconds );
    _creatureFootprints.insert( idCreature, footprint );
    _creatureTypes.insert( idCreature, creatureType );
}

void StreamRenderWorld::startCreatureAttack( int idCreature, int targetX, int targetY, double castSeconds ) {
    auto it = _creatures.find( idCreature );

    if ( it != _creatures.end() ) {
        it->startAttack( targetX, targetY, castSeconds );
    }
}

void StreamRenderWorld::removeCreature( int idCreature ) {
    _creatures.remove( idCreature );
    _creatureFootprints.remove( idCreature );
    _creatureTypes.remove( idCreature );
}

void StreamRenderWorld::addGroundItem( int idGroundItem, const QString& idItem, int x, int y, int z ) {
    GroundItemModel groundItem;
    groundItem.setIdGroundItem( idGroundItem );
    groundItem.setIdItem( idItem );
    groundItem.position().setX( x );
    groundItem.position().setY( y );
    groundItem.position().setZ( z );

    _groundItems.insert( idGroundItem, groundItem );
}

void StreamRenderWorld::removeGroundItem( int idGroundItem ) {
    _groundItems.remove( idGroundItem );
}

int StreamRenderWorld::groundItemAt( int x, int y, int z ) const {
    int topmost = 0;

    for ( auto it = _groundItems.constBegin(); it != _groundItems.constEnd(); ++it ) {
        const EntityPositionModel& position = it->position();

        if ( position.x() == x && position.y() == y && position.z() == z && it.key() > topmost ) {
            topmost = it.key();
        }
    }

    return topmost;
}

void StreamRenderWorld::clearEntities() {
    _ownCharacter.clear();
    _characters.clear();
    _creatures.clear();
    _creatureFootprints.clear();
    _creatureTypes.clear();
    _groundItems.clear();
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

QImage StreamRenderWorld::creatureTexture( int idCreature, const EntityAnimationModel& animation ) const {
    const CreatureTypeModel* creatureType = Singleton<DataManager>::instance().creatureTypeCatalog().creatureType( _creatureTypes.value( idCreature ) );

    if ( !creatureType ) {
        return QImage();
    }

    AppearanceStateEnum state = AppearanceStateEnum::IDLE;
    qint64 elapsedMs = QDateTime::currentMSecsSinceEpoch();

    if ( animation.isAttacking() ) {
        state = AppearanceStateEnum::ATTACK;
        elapsedMs = animation.attackElapsedMs();
    } else if ( animation.isMoving() ) {
        state = AppearanceStateEnum::WALK;
    }

    return creatureType->appearance().animation( state, animation.facing() ).frameAt( elapsedMs );
}

} // namespace Engine
