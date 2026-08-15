#ifndef RENDERWORLD_H
#define RENDERWORLD_H

#include <MMORPGEngine/World/Tile/TileModel.h>

namespace Engine {

class RenderWorld {
public:
    RenderWorld();
    virtual ~RenderWorld();

    virtual const TileModel* tile( int x, int y, int z ) const = 0;
};

} // namespace Engine

#endif // RENDERWORLD_H
