#ifndef RENDERVISIBLEAREA_H
#define RENDERVISIBLEAREA_H

#include <QRect>

#include <MMORPGEngine/Renderer/Camera/Camera.h>
#include <MMORPGEngine/Renderer/World/RenderWorld.h>

namespace Engine {

class RenderVisibleArea {
public:
    RenderVisibleArea( const Camera& camera, const RenderWorld& world );

    bool isEmpty() const;

    int firstChunkX() const;
    int lastChunkX() const;
    int firstChunkY() const;
    int lastChunkY() const;

    QRect tilesInChunk( int chunkX, int chunkY ) const;

private:
    int _firstTileX;
    int _lastTileX;
    int _firstTileY;
    int _lastTileY;
};

} // namespace Engine

#endif // RENDERVISIBLEAREA_H
