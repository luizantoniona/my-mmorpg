#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QColor>
#include <QList>
#include <QMetaObject>
#include <QQuickItem>
#include <QTimer>
#include <QVariantList>

#include <MMORPGEngine/Renderer/Renderer.h>
#include <MMORPGEngine/Renderer/Scene/TextureCache.h>

namespace Engine {

class Viewport : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY( QPointF cameraPosition READ cameraPosition WRITE setCameraPosition NOTIFY cameraPositionChanged )
    Q_PROPERTY( RenderWorld* renderWorld READ renderWorld WRITE setRenderWorld )
    Q_PROPERTY( int activeFloor READ activeFloor WRITE setActiveFloor NOTIFY activeFloorChanged )
    Q_PROPERTY( int cursorShape READ cursorShape WRITE setCursorShape NOTIFY cursorShapeChanged )

public:
    // Generic tile-space rectangle overlay — no domain meaning here, callers decide what it represents.
    class OverlayRect {
    public:
        int x;
        int y;
        int width;
        int height;
        QColor color;
    };

    explicit Viewport( QQuickItem* parent = nullptr );

    QPointF cameraPosition() const;
    void setCameraPosition( const QPointF& position );

    Q_INVOKABLE void centerCameraOnTile( int x, int y );
    Q_INVOKABLE void moveCameraByTiles( int dx, int dy );

    Camera* camera() const;

    RenderWorld* renderWorld() const;
    void setRenderWorld( RenderWorld* world );

    int activeFloor() const;
    void setActiveFloor( int z );

    int cursorShape() const;
    void setCursorShape( int shape );

    Q_INVOKABLE void setHighlightedTile( int x, int y );
    Q_INVOKABLE void clearHighlight();

    Q_INVOKABLE void setOverlayRects( const QVariantList& rects, const QColor& color );
    Q_INVOKABLE void clearOverlayRects();

    // TODO: See if we can remove this method
    Q_INVOKABLE void forceRedraw();

signals:
    void cameraPositionChanged();
    void activeFloorChanged();
    void cursorShapeChanged();
    void tileClicked( int x, int y, int z );
    void tileRightClicked( int x, int y, int z );

protected:
    void geometryChange( const QRectF& newGeometry, const QRectF& oldGeometry ) override;

    void mousePressEvent( QMouseEvent* event ) override;

    QSGNode* updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData ) override;

private:
    void updateWorldBounds();
    void addOverlayNode( QSGNode* parent, const OverlayRect& rect ) const;

private:
    Camera* _camera;
    Renderer* _renderer;
    RenderWorld* _world;
    QMetaObject::Connection _worldBoundsConnection;
    QTimer* _animationTimer;
    TextureCache _textureCache;
    QList<OverlayRect> _overlayRects;
    QList<OverlayRect> _highlightRects;

    int _activeFloor;
};

} // namespace Engine

#endif // VIEWPORT_H
