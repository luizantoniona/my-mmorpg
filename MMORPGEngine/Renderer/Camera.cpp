#include "Camera.h"

namespace Engine {

Camera::Camera() :
    _position( 0.0, 0.0 ),
    _viewportSize( 0.0, 0.0 ),
    _zoom( 1.0 ) {
}

const QPointF& Camera::position() const {
    return _position;
}

void Camera::setPosition( const QPointF& position ) {
    _position = position;
}

const QSizeF& Camera::viewportSize() const {
    return _viewportSize;
}

void Camera::setViewportSize( const QSizeF& size ) {
    _viewportSize = size;
}

double Camera::zoom() const {
    return _zoom;
}

void Camera::setZoom( double zoom ) {
    if ( zoom <= 0.0 ) {
        return;
    }

    _zoom = zoom;
}

QPointF Camera::worldToScreen( const QPointF& worldPosition ) const {
    const QPointF relativePosition = worldPosition - _position;

    return QPointF( relativePosition.x() * _zoom + _viewportSize.width() / 2.0,
                    relativePosition.y() * _zoom + _viewportSize.height() / 2.0 );
}

} // namespace Engine
