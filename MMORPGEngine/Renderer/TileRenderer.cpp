#include "TileRenderer.h"

#include <QColor>

namespace {
constexpr int TILE_SIZE = 10;
constexpr int TILE_GAP = 2;

QColor groundColor( uint16_t groundId ) {
    switch ( groundId ) {
    case 1:
        return Qt::red;
    case 2:
        return Qt::green;
    case 3:
        return Qt::blue;
    case 4:
        return Qt::yellow;
    case 5:
        return Qt::cyan;
    default:
        return Qt::gray;
    }
}

} // namespace

namespace Engine {

TileRenderer::TileRenderer() {
}

void TileRenderer::render( RenderScene& scene, const Camera& camera, const RenderWorld& world ) {
    const QRectF visibleRect = camera.visibleRect();

    const int startX = static_cast<int>( std::floor( visibleRect.left() / TILE_SIZE ) );

    const int startY = static_cast<int>( std::floor( visibleRect.top() / TILE_SIZE ) );

    const int endX = static_cast<int>( std::ceil( visibleRect.right() / TILE_SIZE ) );

    const int endY = static_cast<int>( std::ceil( visibleRect.bottom() / TILE_SIZE ) );

    constexpr int z = 0;

    for ( int y = startY; y <= endY; ++y ) {
        for ( int x = startX; x <= endX; ++x ) {
            const TileModel* tile = world.tile( x, y, z );

            if ( !tile ) {
                continue;
            }

            renderTile( scene, x, y, z, *tile );
        }
    }
}

void TileRenderer::renderTile( RenderScene& scene, int x, int y, int z, const TileModel& tile ) {
    Q_UNUSED( z );

    const uint16_t groundId = tile.groundId();

    if ( tile.groundId() == 0 ) {
        return;
    }

    const QPointF position( x * TILE_SIZE + TILE_GAP, y * TILE_SIZE + TILE_GAP );

    const QSizeF size( TILE_SIZE - TILE_GAP * 2, TILE_SIZE - TILE_GAP * 2 );

    scene.addRect( position, size, groundColor( groundId ) );
}

} // namespace Engine
