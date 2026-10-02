#include "EffectRenderer.h"

#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

namespace {
constexpr double TILE_FLASH_MAX_INSET_RATIO = 0.35;
} // namespace

EffectRenderer::EffectRenderer() :
    _tileFlashes(),
    _clock() {
    _clock.start();
}

void EffectRenderer::addTileFlash( int x, int y, const QColor& color, int durationMs ) {
    appendTileFlash( x, y, color, durationMs, false );
}

void EffectRenderer::addTileWarning( int x, int y, const QColor& color, int durationMs ) {
    appendTileFlash( x, y, color, durationMs, true );
}

void EffectRenderer::render( RenderScene& scene ) {
    const qint64 nowMs = _clock.elapsed();

    for ( int index = _tileFlashes.size() - 1; index >= 0; --index ) {
        const double progress = static_cast<double>( nowMs - _tileFlashes.at( index ).startMs ) / _tileFlashes.at( index ).durationMs;

        if ( progress >= 1.0 ) {
            _tileFlashes.removeAt( index );
            continue;
        }

        renderTileFlash( scene, _tileFlashes.at( index ), progress );
    }
}

void EffectRenderer::appendTileFlash( int x, int y, const QColor& color, int durationMs, bool intensifying ) {
    if ( durationMs <= 0 ) {
        return;
    }

    TileFlash flash;
    flash.x = x;
    flash.y = y;
    flash.color = color;
    flash.startMs = _clock.elapsed();
    flash.durationMs = durationMs;
    flash.intensifying = intensifying;

    _tileFlashes.append( flash );
}

void EffectRenderer::renderTileFlash( RenderScene& scene, const TileFlash& flash, double progress ) const {
    const double tileSize = WorldConstants::TILE_SIZE;

    const double inset = flash.intensifying ? 0.0 : progress * tileSize * TILE_FLASH_MAX_INSET_RATIO;

    const QPointF position( flash.x * tileSize + inset, flash.y * tileSize + inset );
    const QSizeF size( tileSize - 2.0 * inset, tileSize - 2.0 * inset );

    QColor color = flash.color;
    color.setAlphaF( color.alphaF() * ( flash.intensifying ? progress : 1.0 - progress ) );

    scene.addRect( RenderSceneLayerEnum::Effect, position, size, color );
}

} // namespace Engine
