#include "ServerRenderWorld.h"

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>

ServerRenderWorld::ServerRenderWorld( QObject* parent ) :
    Engine::RenderWorld( parent ),
    _world( Engine::Singleton<Server::WorldManager>::instance().world() ) {
}

Engine::WorldModel* ServerRenderWorld::world() const {
    return _world;
}

void ServerRenderWorld::setWorld( Engine::WorldModel* world ) {
    _world = world;

    emit boundsChanged();
}

const Engine::WorldObjectModel* ServerRenderWorld::object( int x, int y, int z ) const {
    if ( !_world ) {
        return nullptr;
    }

    return _world->object( x, y, z );
}

const Engine::WorldTileModel* ServerRenderWorld::tile( int x, int y, int z ) const {
    if ( !_world ) {
        return nullptr;
    }

    return _world->tile( x, y, z );
}

uint32_t ServerRenderWorld::width() const {
    if ( !_world ) {
        return 0;
    }

    return _world->width();
}

uint32_t ServerRenderWorld::height() const {
    if ( !_world ) {
        return 0;
    }

    return _world->height();
}
