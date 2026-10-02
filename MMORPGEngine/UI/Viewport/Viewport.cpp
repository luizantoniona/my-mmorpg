#include "Viewport.h"

#include <cstdlib>

#include <QColor>
#include <QCursor>
#include <QHoverEvent>

#include <MMORPGEngine/Renderer/Camera/Camera.h>
#include <MMORPGEngine/Renderer/Renderer.h>
#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

namespace {
constexpr int ANIMATION_INTERVAL_MS = 16;
constexpr int NO_FOLLOWED_ENTITY = -1;
} // namespace

Viewport::Viewport( QQuickItem* parent ) :
    QQuickItem( parent ),
    _camera( new Camera() ),
    _renderer( new Renderer() ),
    _world( nullptr ),
    _animationTimer( new QTimer( this ) ),
    _hoveredTile( 0, 0 ),
    _lastDraggedTile( 0, 0 ),
    _activeFloor( 0 ),
    _followedEntity( NO_FOLLOWED_ENTITY ),
    _isDragging( false ) {

    setFlag( ItemHasContents, true );

    setAcceptedMouseButtons( Qt::LeftButton | Qt::RightButton );
    setAcceptHoverEvents( true );

    _renderer->initialize();
    _renderer->resize( size() );

    _animationTimer->setInterval( ANIMATION_INTERVAL_MS );
    connect( _animationTimer, &QTimer::timeout, this, [ this ]() {
        updateFollowedCamera();
        update();
    } );
    _animationTimer->start();

    update();
}

QPointF Viewport::cameraPosition() const {
    return _camera->position();
}

void Viewport::setCameraPosition(
    const QPointF& position ) {
    if ( _camera->position() == position ) {
        return;
    }

    _camera->setPosition( position );

    emit cameraPositionChanged();

    update();
}

void Viewport::centerCameraOnTile( int x, int y ) {
    _camera->centerOnTile( x, y );

    emit cameraPositionChanged();

    update();
}

void Viewport::moveCameraByTiles( int dx, int dy ) {
    _camera->moveByTiles( dx, dy );

    emit cameraPositionChanged();

    update();
}

void Viewport::followEntity( int idEntity ) {
    _followedEntity = idEntity;

    updateFollowedCamera();

    update();
}

void Viewport::stopFollowingEntity() {
    _followedEntity = NO_FOLLOWED_ENTITY;
}

Camera* Viewport::camera() const {
    return _camera;
}

RenderWorld* Viewport::renderWorld() const {
    return _world;
}

void Viewport::setRenderWorld( RenderWorld* world ) {
    if ( _world == world ) {
        return;
    }

    QObject::disconnect( _worldBoundsConnection );

    _world = world;

    if ( _world ) {
        _worldBoundsConnection = connect( _world, &RenderWorld::boundsChanged, this, &Viewport::updateWorldBounds );
    }

    updateWorldBounds();

    update();
}

void Viewport::updateWorldBounds() {
    const double tileSize = WorldConstants::TILE_SIZE;
    _camera->setWorldSize( _world ? QSizeF( _world->width() * tileSize, _world->height() * tileSize )
                                  : QSizeF( 0.0, 0.0 ) );

    emit cameraPositionChanged();
}

void Viewport::updateFollowedCamera() {
    if ( _followedEntity == NO_FOLLOWED_ENTITY || !_world ) {
        return;
    }

    const double tileSize = WorldConstants::TILE_SIZE;

    for ( const RenderWorld::Entity& entity : _world->entities( _activeFloor ) ) {
        if ( entity.idEntity != _followedEntity ) {
            continue;
        }

        const QPointF previousPosition = _camera->position();

        _camera->setPosition( QPointF( entity.x * tileSize + entity.offsetX + tileSize / 2.0,
                                       entity.y * tileSize + entity.offsetY + tileSize / 2.0 ) );

        if ( _camera->position() != previousPosition ) {
            emit cameraPositionChanged();
        }

        return;
    }
}

int Viewport::activeFloor() const {
    return _activeFloor;
}

void Viewport::setActiveFloor( int z ) {
    if ( _activeFloor == z ) {
        return;
    }

    _activeFloor = z;

    emit activeFloorChanged();

    update();
}

int Viewport::cursorShape() const {
    return cursor().shape();
}

void Viewport::setCursorShape( int shape ) {
    if ( cursor().shape() == static_cast<Qt::CursorShape>( shape ) ) {
        return;
    }

    setCursor( QCursor( static_cast<Qt::CursorShape>( shape ) ) );

    emit cursorShapeChanged();
}

QPoint Viewport::hoveredTile() const {
    return _hoveredTile;
}

void Viewport::setHighlightedTile( int x, int y ) {
    _renderer->overlayRenderer()->setHighlightedTile( x, y );

    update();
}

void Viewport::clearHighlight() {
    _renderer->overlayRenderer()->clearHighlight();

    update();
}

void Viewport::setOverlayRects( const QVariantList& rects, const QColor& color ) {
    _renderer->overlayRenderer()->setOverlayRects( rects, color );

    update();
}

void Viewport::clearOverlayRects() {
    _renderer->overlayRenderer()->clearOverlayRects();

    update();
}

void Viewport::addTileFlash( int x, int y, const QColor& color, int durationMs ) {
    _renderer->effectRenderer()->addTileFlash( x, y, color, durationMs );

    update();
}

void Viewport::addTileWarning( int x, int y, const QColor& color, int durationMs ) {
    _renderer->effectRenderer()->addTileWarning( x, y, color, durationMs );

    update();
}

void Viewport::geometryChange( const QRectF& newGeometry, const QRectF& oldGeometry ) {
    QQuickItem::geometryChange( newGeometry, oldGeometry );
    _camera->setViewportSize( newGeometry.size() );
    _renderer->resize( newGeometry.size() );

    emit cameraPositionChanged();
}

void Viewport::mousePressEvent( QMouseEvent* event ) {
    if ( !_world ) {
        return;
    }

    // TODO: WheelButton
    if ( event->button() != Qt::LeftButton && event->button() != Qt::RightButton ) {
        QQuickItem::mousePressEvent( event );
        return;
    }

    const QPoint tile = screenToTile( event->position() );
    const int x = tile.x();
    const int y = tile.y();
    const int z = _activeFloor;

    if ( !isInsideWorld( tile ) ) {
        return;
    }

    if ( event->button() == Qt::RightButton ) {
        emit tileRightClicked( x, y, z );
        return;
    }

    _isDragging = true;
    _lastDraggedTile = tile;

    emit tileClicked( x, y, z );
}

void Viewport::mouseMoveEvent( QMouseEvent* event ) {
    if ( !_world ) {
        return;
    }

    const QPoint tile = screenToTile( event->position() );

    if ( tile != _hoveredTile ) {
        _hoveredTile = tile;

        emit hoveredTileChanged();
    }

    if ( !_isDragging || tile == _lastDraggedTile ) {
        return;
    }

    int x = _lastDraggedTile.x();
    int y = _lastDraggedTile.y();
    const int deltaX = std::abs( tile.x() - x );
    const int deltaY = std::abs( tile.y() - y );
    const int stepX = x < tile.x() ? 1 : -1;
    const int stepY = y < tile.y() ? 1 : -1;
    int error = deltaX - deltaY;

    while ( x != tile.x() || y != tile.y() ) {
        const int doubledError = 2 * error;

        if ( doubledError > -deltaY ) {
            error -= deltaY;
            x += stepX;
        }

        if ( doubledError < deltaX ) {
            error += deltaX;
            y += stepY;
        }

        if ( isInsideWorld( QPoint( x, y ) ) ) {
            emit tileDragged( x, y, _activeFloor );
        }
    }

    _lastDraggedTile = tile;
}

void Viewport::mouseReleaseEvent( QMouseEvent* event ) {
    if ( event->button() == Qt::LeftButton ) {
        _isDragging = false;
    }
}

void Viewport::hoverMoveEvent( QHoverEvent* event ) {
    const QPoint tile = screenToTile( event->position() );

    if ( tile == _hoveredTile ) {
        return;
    }

    _hoveredTile = tile;

    emit hoveredTileChanged();
}

QPoint Viewport::screenToTile( const QPointF& screenPosition ) const {
    const QPointF worldPosition = _camera->screenToWorld( screenPosition );

    const double tileSize = WorldConstants::TILE_SIZE;

    const int x = static_cast<int>( std::floor( worldPosition.x() / tileSize ) );
    const int y = static_cast<int>( std::floor( worldPosition.y() / tileSize ) );

    return QPoint( x, y );
}

bool Viewport::isInsideWorld( const QPoint& tile ) const {
    return _world && tile.x() >= 0 && tile.y() >= 0 && tile.x() < static_cast<int>( _world->width() ) && tile.y() < static_cast<int>( _world->height() );
}

QSGNode* Viewport::updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* ) {
    delete oldNode;

    auto* rootNode = new QSGNode();

    RenderScene scene;

    if ( !_world || width() <= 0.0 || height() <= 0.0 ) {
        return rootNode;
    }

    _renderer->render( scene, *_camera, *_world, _activeFloor );

    scene.build( rootNode, window(), *_camera, _textureCache );

    return rootNode;
}

} // namespace Engine
