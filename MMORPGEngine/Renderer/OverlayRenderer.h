#ifndef OVERLAYRENDERER_H
#define OVERLAYRENDERER_H

#include <QColor>
#include <QList>
#include <QVariantList>

#include <MMORPGEngine/Renderer/Scene/RenderScene.h>

namespace Engine {

class OverlayRenderer {
public:
    OverlayRenderer();

    void setHighlightedTile( int x, int y );
    void clearHighlight();

    void setOverlayRects( const QVariantList& rects, const QColor& color );
    void clearOverlayRects();

    void render( RenderScene& scene );

private:
    class OverlayRect {
    public:
        int x;
        int y;
        int width;
        int height;
        QColor color;
    };

    void renderRect( RenderScene& scene, const OverlayRect& rect ) const;

private:
    QList<OverlayRect> _overlayRects;
    QList<OverlayRect> _highlightRects;
};

} // namespace Engine

#endif // OVERLAYRENDERER_H
