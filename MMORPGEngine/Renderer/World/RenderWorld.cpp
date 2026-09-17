#include "RenderWorld.h"

namespace Engine {

RenderWorld::RenderWorld( QObject* parent ) :
    QObject( parent ) {
}

bool RenderWorld::hasObject( int x, int y, int z ) const {
    return object( x, y, z ) != nullptr;
}

bool RenderWorld::hasTile( int x, int y, int z ) const {
    return tile( x, y, z ) != nullptr;
}

int RenderWorld::resolveFloor( int x, int y, int z ) const {
    return resolveFloor( x, y, z, floors() );
}

int RenderWorld::resolveFloor( int x, int y, int z, const std::vector<int>& loadedFloors ) const {
    const WorldTileModel* activeTile = tile( x, y, z );
    if ( activeTile && activeTile->tileModel() ) {
        return z;
    }

    for ( auto iterator = loadedFloors.rbegin(); iterator != loadedFloors.rend(); ++iterator ) {
        const int floor = *iterator;

        if ( floor >= z ) {
            continue;
        }

        const WorldTileModel* candidateTile = tile( x, y, floor );
        if ( candidateTile && candidateTile->tileModel() ) {
            return floor;
        }
    }

    return z;
}

} // namespace Engine
