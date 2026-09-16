#include "ClientRenderWorld.h"

ClientRenderWorld::ClientRenderWorld( QObject* parent ) :
    Engine::RenderWorld( parent ),
    _world( nullptr ),
    _entities() {
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

    for ( auto it = _entities.constBegin(); it != _entities.constEnd(); ++it ) {
        if ( it->z != z ) {
            continue;
        }

        result.append( Engine::RenderWorld::Entity( it.key(), it->x, it->y ) );
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

void ClientRenderWorld::setEntity( int idEntity, int x, int y, int z, const QString& orientation ) {
    _entities.insert( idEntity, EntityPosition( x, y, z, orientation ) );
}

void ClientRenderWorld::removeEntity( int idEntity ) {
    _entities.remove( idEntity );
}

void ClientRenderWorld::clearEntities() {
    _entities.clear();
}
