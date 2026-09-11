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

} // namespace Engine
