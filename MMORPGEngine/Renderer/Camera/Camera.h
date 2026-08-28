#ifndef CAMERA_H
#define CAMERA_H

#include <QPointF>
#include <QRectF>
#include <QSizeF>

namespace Engine {

class Camera {
public:
    Camera();

    const QPointF& position() const;
    void setPosition( const QPointF& position );
    void centerOnTile( int x, int y );
    void moveByTiles( int dx, int dy );

    const QSizeF& viewportSize() const;
    void setViewportSize( const QSizeF& size );

    void setZoom( double zoom );
    double zoom() const;

    QRectF visibleRect() const;

    QPointF worldToScreen( const QPointF& worldPosition ) const;

    QPointF screenToWorld( const QPointF& screenPosition ) const;

private:
    QPointF _position;
    QSizeF _viewportSize;
    double _zoom;
};

} // namespace Engine

#endif // CAMERA_H
