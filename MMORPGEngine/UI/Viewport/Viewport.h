#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QColor>
#include <QMetaObject>
#include <QPoint>
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
    Q_PROPERTY( QPoint hoveredTile READ hoveredTile NOTIFY hoveredTileChanged )

public:
    explicit Viewport( QQuickItem* parent = nullptr );

    QPointF cameraPosition() const;
    void setCameraPosition( const QPointF& position );

    Q_INVOKABLE void centerCameraOnTile( int x, int y );
    Q_INVOKABLE void moveCameraByTiles( int dx, int dy );

    Q_INVOKABLE void followEntity( int idEntity );
    Q_INVOKABLE void stopFollowingEntity();

    Camera* camera() const;

    RenderWorld* renderWorld() const;
    void setRenderWorld( RenderWorld* world );

    int activeFloor() const;
    void setActiveFloor( int z );

    int cursorShape() const;
    void setCursorShape( int shape );

    QPoint hoveredTile() const;

    Q_INVOKABLE void setHighlightedTile( int x, int y );
    Q_INVOKABLE void clearHighlight();

    Q_INVOKABLE void setOverlayRects( const QVariantList& rects, const QColor& color );
    Q_INVOKABLE void clearOverlayRects();

    Q_INVOKABLE void addTileFlash( int x, int y, const QColor& color, int durationMs );
    Q_INVOKABLE void addTileWarning( int x, int y, const QColor& color, int durationMs );

signals:
    void cameraPositionChanged();
    void activeFloorChanged();
    void cursorShapeChanged();
    void hoveredTileChanged();
    void tileClicked( int x, int y, int z );
    void tileRightClicked( int x, int y, int z );

protected:
    void geometryChange( const QRectF& newGeometry, const QRectF& oldGeometry ) override;

    void mousePressEvent( QMouseEvent* event ) override;
    void hoverMoveEvent( QHoverEvent* event ) override;

    QSGNode* updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData ) override;

private:
    void updateWorldBounds();
    void updateFollowedCamera();
    QPoint screenToTile( const QPointF& screenPosition ) const;

private:
    Camera* _camera;
    Renderer* _renderer;
    RenderWorld* _world;
    QMetaObject::Connection _worldBoundsConnection;
    QTimer* _animationTimer;
    TextureCache _textureCache;
    QPoint _hoveredTile;

    int _activeFloor;
    int _followedEntity;
};

} // namespace Engine

#endif // VIEWPORT_H
