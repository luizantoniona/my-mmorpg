#include "TileRenderer.h"

#include <QColor>
#include <QDateTime>

#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

TileRenderer::TileRenderer() {
}

void TileRenderer::render( RenderScene& scene, const Camera& camera, const RenderWorld& world, int z ) {
    const QRectF visibleRect = camera.visibleRect();

    const int startX = static_cast<int>( std::floor( visibleRect.left() / WorldConstants::TILE_SIZE ) );
    const int startY = static_cast<int>( std::floor( visibleRect.top() / WorldConstants::TILE_SIZE ) );
    const int endX = static_cast<int>( std::ceil( visibleRect.right() / WorldConstants::TILE_SIZE ) );
    const int endY = static_cast<int>( std::ceil( visibleRect.bottom() / WorldConstants::TILE_SIZE ) );

    const qint64 elapsedMs = QDateTime::currentMSecsSinceEpoch();
    const std::vector<int> loadedFloors = world.floors();

    for ( int y = startY; y <= endY; ++y ) {
        for ( int x = startX; x <= endX; ++x ) {
            const int effectiveZ = world.resolveFloor( x, y, z, loadedFloors );
            const WorldTileModel* tile = world.tile( x, y, effectiveZ );

            if ( !tile || !tile->tileModel() ) {
                continue;
            }

            renderTile( scene, x, y, effectiveZ, *tile, elapsedMs );

            if ( effectiveZ != z ) {
                const QPointF position( x * WorldConstants::TILE_SIZE, y * WorldConstants::TILE_SIZE );
                const QSizeF size( WorldConstants::TILE_SIZE, WorldConstants::TILE_SIZE );

                scene.addRect( position, size, QColor( 0, 0, 0, 130 ) );
            }
        }
    }
}

void TileRenderer::renderTile( RenderScene& scene, int x, int y, int z, const WorldTileModel& worldTile, qint64 elapsedMs ) {
    Q_UNUSED( z );

    const QImage frame = worldTile.tileModel()->animation().frameAt( elapsedMs );
    if ( frame.isNull() ) {
        return;
    }

    const QPointF position( x * WorldConstants::TILE_SIZE, y * WorldConstants::TILE_SIZE );
    const QSizeF size( WorldConstants::TILE_SIZE, WorldConstants::TILE_SIZE );

    scene.addTexture( position, size, frame );
}

} // namespace Engine
