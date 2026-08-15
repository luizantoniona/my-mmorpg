#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QQuickItem>

#include <MMORPGEngine/Renderer/Renderer.h>

namespace Engine {

class Viewport : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY( QPointF cameraPosition READ cameraPosition WRITE setCameraPosition NOTIFY cameraPositionChanged )

public:
    explicit Viewport( QQuickItem* parent = nullptr );

    QPointF cameraPosition() const;
    void setCameraPosition( const QPointF& position );

    Camera* camera() const;

    void setRenderer( Renderer* renderer );

signals:
    void cameraPositionChanged();

protected:
    void geometryChange( const QRectF& newGeometry, const QRectF& oldGeometry ) override;

    QSGNode* updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData ) override;

private:
    Camera* _camera;
    Renderer* _renderer;
};

} // namespace Engine

#endif // VIEWPORT_H
