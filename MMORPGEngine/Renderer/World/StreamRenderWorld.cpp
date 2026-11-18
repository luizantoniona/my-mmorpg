#include "StreamRenderWorld.h"

#include <QDateTime>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Character/CharacterAppearanceFactory.h>
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

    appendCharacters( result, _ownCharacter, z );
    appendCharacters( result, _characters, z );

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

void StreamRenderWorld::startCharacterAttack( int idCharacter, int targetX, int targetY, double castSeconds ) {
    auto own = _ownCharacter.find( idCharacter );

    if ( own != _ownCharacter.end() ) {
        own->startAttack( targetX, targetY, castSeconds );
        return;
    }

    auto other = _characters.find( idCharacter );

    if ( other != _characters.end() ) {
        other->startAttack( targetX, targetY, castSeconds );
    }
}

void StreamRenderWorld::removeCharacter( int idCharacter ) {
    _characters.remove( idCharacter );
    _characterAppearances.remove( idCharacter );
}

void StreamRenderWorld::setCharacterAppearance( int idCharacter, const CharacterAppearanceModel& characterAppearance ) {
    _characterAppearances.insert( idCharacter, characterAppearance );
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
    _characterAppearances.clear();
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

QImage StreamRenderWorld::frameOf( const AppearanceModel& appearance, const EntityAnimationModel& animation ) {
    AppearanceStateEnum state = AppearanceStateEnum::IDLE;
    qint64 elapsedMs = QDateTime::currentMSecsSinceEpoch();

    if ( animation.isAttacking() ) {
        state = AppearanceStateEnum::ATTACK;
        elapsedMs = animation.attackElapsedMs();
    } else if ( animation.isMoving() ) {
        state = AppearanceStateEnum::WALK;
    }

    return appearance.animation( state, animation.facing() ).frameAt( elapsedMs );
}

void StreamRenderWorld::appendCharacters( QList<Entity>& result, const QHash<int, EntityAnimationModel>& characters, int z ) const {
    for ( auto it = characters.constBegin(); it != characters.constEnd(); ++it ) {
        if ( it->z() != z ) {
            continue;
        }

        QImage texture;

        if ( const AppearanceModel* appearance = characterAppearance( it.key() ) ) {
            texture = frameOf( *appearance, it.value() );
        }

        if ( texture.isNull() ) {
            texture = EntityTextureModel::characterTexture();
        }

        Entity entity( it.key(), it->x(), it->y(), texture );
        entity.offsetX = it->offsetX();
        entity.offsetY = it->offsetY();

        result.append( entity );
    }
}

const AppearanceModel* StreamRenderWorld::characterAppearance( int idCharacter ) const {
    const CharacterCatalog& characterCatalog = Singleton<DataManager>::instance().characterCatalog();

    const CharacterAppearanceModel selection = _characterAppearances.value( idCharacter );

    if ( !characterCatalog.isValidAppearance( selection ) ) {
        return nullptr;
    }

    auto it = _composedAppearances.find( selection.key() );

    if ( it == _composedAppearances.end() ) {
        it = _composedAppearances.insert( selection.key(), CharacterAppearanceFactory::createAppearance( characterCatalog, selection ) );
    }

    return &it.value();
}

QImage StreamRenderWorld::creatureTexture( int idCreature, const EntityAnimationModel& animation ) const {
    const CreatureTypeModel* creatureType = Singleton<DataManager>::instance().creatureTypeCatalog().creatureType( _creatureTypes.value( idCreature ) );

    if ( !creatureType ) {
        return QImage();
    }

    return frameOf( creatureType->appearance(), animation );
}

} // namespace Engine
