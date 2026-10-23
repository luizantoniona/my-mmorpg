#include "RenderVisibleArea.h"

#include <algorithm>
#include <cmath>

#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

RenderVisibleArea::RenderVisibleArea( const Camera& camera, const RenderWorld& world ) :
    _firstTileX( 0 ),
    _lastTileX( -1 ),
    _firstTileY( 0 ),
    _lastTileY( -1 ) {

    const int worldWidth = static_cast<int>( world.width() );
    const int worldHeight = static_cast<int>( world.height() );

    if ( worldWidth <= 0 || worldHeight <= 0 ) {
        return;
    }

    const QRectF visibleRect = camera.visibleRect();
    const double tileSize = WorldConstants::TILE_SIZE;

    _firstTileX = std::max( 0, static_cast<int>( std::floor( visibleRect.left() / tileSize ) ) );
    _lastTileX = std::min( worldWidth - 1, static_cast<int>( std::ceil( visibleRect.right() / tileSize ) ) );
    _firstTileY = std::max( 0, static_cast<int>( std::floor( visibleRect.top() / tileSize ) ) );
    _lastTileY = std::min( worldHeight - 1, static_cast<int>( std::ceil( visibleRect.bottom() / tileSize ) ) );
}

bool RenderVisibleArea::isEmpty() const {
    return _firstTileX > _lastTileX || _firstTileY > _lastTileY;
}

int RenderVisibleArea::firstChunkX() const {
    return _firstTileX / WorldConstants::CHUNK_SIZE;
}

int RenderVisibleArea::lastChunkX() const {
    return _lastTileX / WorldConstants::CHUNK_SIZE;
}

int RenderVisibleArea::firstChunkY() const {
    return _firstTileY / WorldConstants::CHUNK_SIZE;
}

int RenderVisibleArea::lastChunkY() const {
    return _lastTileY / WorldConstants::CHUNK_SIZE;
}

QRect RenderVisibleArea::tilesInChunk( int chunkX, int chunkY ) const {
    const int chunkSize = WorldConstants::CHUNK_SIZE;

    const int firstX = std::max( _firstTileX, chunkX * chunkSize );
    const int lastX = std::min( _lastTileX, chunkX * chunkSize + chunkSize - 1 );
    const int firstY = std::max( _firstTileY, chunkY * chunkSize );
    const int lastY = std::min( _lastTileY, chunkY * chunkSize + chunkSize - 1 );

    return QRect( QPoint( firstX, firstY ), QPoint( lastX, lastY ) );
}

} // namespace Engine
