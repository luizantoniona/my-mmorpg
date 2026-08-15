#include "ServerRenderWorld.h"

ServerRenderWorld::ServerRenderWorld( QObject* parent ) :
    Engine::RenderWorld( parent ) {

    for ( int y = 0; y < 20; ++y ) {
        for ( int x = 0; x < 20; ++x ) {
            Engine::TileModel tile;

            tile.setGroundId( 1 );

            _tiles.insert( key( x, y, 0 ), tile );
        }
    }
}

const Engine::TileModel* ServerRenderWorld::tile( int x, int y, int z ) const {
    const auto iterator = _tiles.constFind( key( x, y, z ) );

    if ( iterator == _tiles.constEnd() ) {
        return nullptr;
    }

    return &iterator.value();
}

QString ServerRenderWorld::key( int x, int y, int z ) const {
    return QString( "%1:%2:%3" ).arg( x ).arg( y ).arg( z );
}
