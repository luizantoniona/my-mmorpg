#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QQuickItem>

#include <MMORPGEngine/Renderer/Renderer.h>

namespace Engine {

class Viewport : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY( QPointF cameraPosition READ cameraPosition WRITE setCameraPosition NOTIFY cameraPositionChanged )
    Q_PROPERTY( RenderWorld* renderWorld READ renderWorld WRITE setRenderWorld )

public:
    explicit Viewport( QQuickItem* parent = nullptr );

    QPointF cameraPosition() const;
    void setCameraPosition( const QPointF& position );
    Q_INVOKABLE void centerCameraOnTile( int x, int y );
    Q_INVOKABLE void moveCameraByTiles( int dx, int dy );

    Camera* camera() const;

    RenderWorld* renderWorld() const;
    void setRenderWorld( RenderWorld* world );

signals:
    void cameraPositionChanged();
    void tileClicked( int x, int y, int z );

protected:
    void geometryChange( const QRectF& newGeometry, const QRectF& oldGeometry ) override;

    void mousePressEvent( QMouseEvent* event ) override;

    QSGNode* updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData ) override;

private:
    Camera* _camera;
    Renderer* _renderer;
    RenderWorld* _world;
};

} // namespace Engine

#endif // VIEWPORT_H
