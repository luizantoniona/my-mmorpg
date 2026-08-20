#include "ServerRenderWorld.h"

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>

ServerRenderWorld::ServerRenderWorld( QObject* parent ) :
    Engine::RenderWorld( parent ),
    _data( Engine::Singleton<Engine::DataManager>::instance() ),
    _world( Engine::Singleton<Server::WorldManager>::instance().world() ) {
}

Engine::WorldModel* ServerRenderWorld::world() const {
    return _world;
}

void ServerRenderWorld::setWorld( Engine::WorldModel* world ) {
    _world = world;
}

const Engine::GroundModel* ServerRenderWorld::ground( uint32_t id ) const {
    return _data.groundCatalog().ground( id );
}

const Engine::TileModel* ServerRenderWorld::tile( int x, int y, int z ) const {
    if ( !_world ) {
        return nullptr;
    }

    return _world->tile( x, y, z );
}
