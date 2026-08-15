#include "TileRenderer.h"

namespace {
constexpr int TILE_SIZE = 10;
constexpr int TILE_GAP = 2;
} // namespace

namespace Engine {

TileRenderer::TileRenderer() {
}

void TileRenderer::render( RenderScene& scene ) {
    constexpr int width = 10;
    constexpr int height = 10;

    for ( int y = 0; y < height; ++y ) {
        for ( int x = 0; x < width; ++x ) {
            renderTile( scene, x, y );
        }
    }
}

void TileRenderer::renderTile( RenderScene& scene, int x, int y ) {
    const QPointF position( x * TILE_SIZE, y * TILE_SIZE );
    scene.addRect( position, QSizeF( TILE_SIZE - TILE_GAP, TILE_SIZE - TILE_GAP ) );
}

} // namespace Engine
