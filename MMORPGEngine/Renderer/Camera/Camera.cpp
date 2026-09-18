#include "Camera.h"

#include <QtGlobal>

#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

Camera::Camera() :
    _position( 0.0, 0.0 ),
    _viewportSize( 0.0, 0.0 ),
    _worldSize( 0.0, 0.0 ),
    _zoom( 1.0 ) {
}

const QPointF& Camera::position() const {
    return _position;
}

void Camera::setPosition( const QPointF& position ) {
    _position = position;
    clampPosition();
}

void Camera::centerOnTile( int x, int y ) {
    const double tileSize = WorldConstants::TILE_SIZE;
    setPosition( QPointF( x * tileSize + tileSize / 2.0, y * tileSize + tileSize / 2.0 ) );
}

void Camera::moveByTiles( int dx, int dy ) {
    const double tileSize = WorldConstants::TILE_SIZE;
    setPosition( _position + QPointF( dx * tileSize, dy * tileSize ) );
}

const QSizeF& Camera::viewportSize() const {
    return _viewportSize;
}

void Camera::setViewportSize( const QSizeF& size ) {
    _viewportSize = size;
    clampPosition();
}

double Camera::zoom() const {
    return _zoom;
}

void Camera::setZoom( double zoom ) {
    if ( zoom <= 0.0 ) {
        return;
    }

    _zoom = zoom;
    clampPosition();
}

const QSizeF& Camera::worldSize() const {
    return _worldSize;
}

void Camera::setWorldSize( const QSizeF& size ) {
    _worldSize = size;
    clampPosition();
}

void Camera::clampPosition() {
    if ( _worldSize.isEmpty() || _viewportSize.isEmpty() ) {
        return;
    }

    const double halfWidth = _viewportSize.width() / ( 2.0 * _zoom );
    const double halfHeight = _viewportSize.height() / ( 2.0 * _zoom );

    double x = _position.x();
    double y = _position.y();

    if ( _worldSize.width() <= halfWidth * 2.0 ) {
        x = _worldSize.width() / 2.0;
    } else {
        x = qBound( halfWidth, x, _worldSize.width() - halfWidth );
    }

    if ( _worldSize.height() <= halfHeight * 2.0 ) {
        y = _worldSize.height() / 2.0;
    } else {
        y = qBound( halfHeight, y, _worldSize.height() - halfHeight );
    }

    _position = QPointF( x, y );
}

QRectF Camera::visibleRect() const {
    const double width = _viewportSize.width() / _zoom;
    const double height = _viewportSize.height() / _zoom;

    return QRectF( _position.x() - width / 2.0, _position.y() - height / 2.0, width, height );
}

QPointF Camera::worldToScreen( const QPointF& worldPosition ) const {
    const QPointF relativePosition = worldPosition - _position;

    return QPointF( relativePosition.x() * _zoom + _viewportSize.width() / 2.0,
                    relativePosition.y() * _zoom + _viewportSize.height() / 2.0 );
}

QPointF Camera::screenToWorld( const QPointF& screenPosition ) const {
    const QPointF relativePosition( screenPosition.x() - _viewportSize.width() / 2.0,
                                    screenPosition.y() - _viewportSize.height() / 2.0 );

    return QPointF( relativePosition.x() / _zoom + _position.x(),
                    relativePosition.y() / _zoom + _position.y() );
}

} // namespace Engine
