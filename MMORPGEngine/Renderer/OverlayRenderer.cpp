#include "OverlayRenderer.h"

#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

OverlayRenderer::OverlayRenderer() :
    _overlayRects(),
    _highlightRects() {
}

void OverlayRenderer::setHighlightedTile( int x, int y ) {
    OverlayRect rect;
    rect.x = x;
    rect.y = y;
    rect.width = 1;
    rect.height = 1;
    rect.color = QColor( 255, 255, 255, 70 );

    _highlightRects = { rect };
}

void OverlayRenderer::clearHighlight() {
    _highlightRects.clear();
}

void OverlayRenderer::setOverlayRects( const QVariantList& rects, const QColor& color ) {
    _overlayRects.clear();
    _overlayRects.reserve( rects.size() );

    for ( const QVariant& rectVariant : rects ) {
        const QVariantMap rectMap = rectVariant.toMap();

        OverlayRect rect;
        rect.x = rectMap.value( "x" ).toInt();
        rect.y = rectMap.value( "y" ).toInt();
        rect.width = rectMap.value( "width" ).toInt();
        rect.height = rectMap.value( "height" ).toInt();
        rect.color = color;

        _overlayRects.append( rect );
    }
}

void OverlayRenderer::clearOverlayRects() {
    _overlayRects.clear();
}

void OverlayRenderer::render( RenderScene& scene ) {
    for ( const OverlayRect& rect : _highlightRects ) {
        renderRect( scene, rect );
    }

    for ( const OverlayRect& rect : _overlayRects ) {
        renderRect( scene, rect );
    }
}

void OverlayRenderer::renderRect( RenderScene& scene, const OverlayRect& rect ) const {
    const double tileSize = WorldConstants::TILE_SIZE;

    const QPointF position( rect.x * tileSize, rect.y * tileSize );
    const QSizeF size( rect.width * tileSize, rect.height * tileSize );

    scene.addRect( RenderSceneLayerEnum::Overlay, position, size, rect.color );
}

} // namespace Engine
