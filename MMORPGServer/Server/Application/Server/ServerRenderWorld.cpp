#include "ServerRenderWorld.h"

ServerRenderWorld::ServerRenderWorld( Engine::WorldModel* world, QObject* parent ) :
    Engine::RenderWorld( parent ),
    _world( world ) {
}

const Engine::TileModel* ServerRenderWorld::tile( int x, int y, int z ) const {
    if ( !_world ) {
        return nullptr;
    }

    return _world->tile( x, y, z );
}
