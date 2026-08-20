#include "TileRenderer.h"

#include <QColor>

namespace {
constexpr int TILE_SIZE = 32;
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

            const TileTextureModel* ground = world.tileTexture( tile->tileTextureId() );

            if ( !ground ) {
                continue;
            }

            renderTile( scene, x, y, z, *tile, *ground );
        }
    }
}

void TileRenderer::renderTile( RenderScene& scene, int x, int y, int z, const TileModel& tile, const TileTextureModel& tileTexture ) {
    Q_UNUSED( z );

    const QPointF position( x * TILE_SIZE, y * TILE_SIZE );

    const QSizeF size( TILE_SIZE, TILE_SIZE );

    if ( tileTexture.texture().isNull() ) {
        return;
    }

    scene.addTexture( position, size, tileTexture.texture() );
}

} // namespace Engine
