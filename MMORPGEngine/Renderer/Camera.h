#ifndef CAMERA_H
#define CAMERA_H

#include <QPointF>
#include <QSizeF>

namespace Engine {

class Camera {
public:
    Camera();

    void setPosition( const QPointF& position );
    void setViewportSize( const QSizeF& size );
    void setZoom( double zoom );

    const QPointF& position() const;
    const QSizeF& viewportSize() const;
    double zoom() const;

    QPointF worldToScreen( const QPointF& worldPosition ) const;

private:
    QPointF _position;
    QSizeF _viewportSize;
    double _zoom;
};

} // namespace Engine

#endif // CAMERA_H
