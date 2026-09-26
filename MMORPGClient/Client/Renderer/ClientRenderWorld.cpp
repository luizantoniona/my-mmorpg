#include "ClientRenderWorld.h"

#include <MMORPGEngine/Renderer/EntityTextureModel.h>

ClientRenderWorld::ClientRenderWorld( QObject* parent ) :
    Engine::RenderWorld( parent ),
    _world( nullptr ),
    _ownCharacter(),
    _characters(),
    _creatures() {
}

Engine::WorldModel* ClientRenderWorld::world() const {
    return _world;
}

void ClientRenderWorld::setWorld( Engine::WorldModel* world ) {
    _world = world;

    emit boundsChanged();
}

const Engine::WorldObjectModel* ClientRenderWorld::object( int x, int y, int z ) const {
    if ( !_world ) {
        return nullptr;
    }

    return _world->object( x, y, z );
}

const Engine::WorldTileModel* ClientRenderWorld::tile( int x, int y, int z ) const {
    if ( !_world ) {
        return nullptr;
    }

    return _world->tile( x, y, z );
}

QList<Engine::RenderWorld::Entity> ClientRenderWorld::entities( int z ) const {
    QList<Engine::RenderWorld::Entity> result;

    for ( auto it = _ownCharacter.constBegin(); it != _ownCharacter.constEnd(); ++it ) {
        if ( it->z != z ) {
            continue;
        }

        result.append( Engine::RenderWorld::Entity( it.key(), it->x, it->y, Engine::EntityTextureModel::characterTexture() ) );
    }

    for ( auto it = _characters.constBegin(); it != _characters.constEnd(); ++it ) {
        if ( it->z != z ) {
            continue;
        }

        result.append( Engine::RenderWorld::Entity( it.key(), it->x, it->y, Engine::EntityTextureModel::characterTexture() ) );
    }

    for ( auto it = _creatures.constBegin(); it != _creatures.constEnd(); ++it ) {
        if ( it->z != z ) {
            continue;
        }

        result.append( Engine::RenderWorld::Entity( it.key(), it->x, it->y, Engine::EntityTextureModel::creatureTexture() ) );
    }

    return result;
}

std::vector<int> ClientRenderWorld::floors() const {
    if ( !_world ) {
        return {};
    }

    return _world->floors();
}

uint32_t ClientRenderWorld::width() const {
    if ( !_world ) {
        return 0;
    }

    return _world->width();
}

uint32_t ClientRenderWorld::height() const {
    if ( !_world ) {
        return 0;
    }

    return _world->height();
}

void ClientRenderWorld::setOwnCharacter( int idCharacter, int x, int y, int z ) {
    _ownCharacter.clear();
    _ownCharacter.insert( idCharacter, EntityPosition( x, y, z ) );
}

void ClientRenderWorld::addCharacter( int idCharacter, int x, int y, int z ) {
    _characters.insert( idCharacter, EntityPosition( x, y, z ) );
}

void ClientRenderWorld::removeCharacter( int idCharacter ) {
    _characters.remove( idCharacter );
}

void ClientRenderWorld::addCreature( int idCreature, int x, int y, int z ) {
    _creatures.insert( idCreature, EntityPosition( x, y, z ) );
}

void ClientRenderWorld::removeCreature( int idCreature ) {
    _creatures.remove( idCreature );
}

void ClientRenderWorld::clearEntities() {
    _ownCharacter.clear();
    _characters.clear();
    _creatures.clear();
}
