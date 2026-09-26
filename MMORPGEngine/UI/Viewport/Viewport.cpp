#include "Viewport.h"

#include <QColor>
#include <QCursor>
#include <QHoverEvent>
#include <QSGSimpleRectNode>

#include <MMORPGEngine/Renderer/Camera/Camera.h>
#include <MMORPGEngine/Renderer/Renderer.h>
#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

namespace {
constexpr int ANIMATION_INTERVAL_MS = 100;
} // namespace

Viewport::Viewport( QQuickItem* parent ) :
    QQuickItem( parent ),
    _camera( new Camera() ),
    _renderer( new Renderer() ),
    _world( nullptr ),
    _animationTimer( new QTimer( this ) ),
    _overlayRects(),
    _highlightRects(),
    _hoveredTile( 0, 0 ),
    _activeFloor( 0 ) {

    setFlag( ItemHasContents, true );

    setAcceptedMouseButtons( Qt::LeftButton | Qt::RightButton );
    setAcceptHoverEvents( true );

    _renderer->initialize();
    _renderer->resize( size() );

    _animationTimer->setInterval( ANIMATION_INTERVAL_MS );
    connect( _animationTimer, &QTimer::timeout, this, [ this ]() {
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
    if ( !_highlightRects.isEmpty() && _highlightRects.first().x == x && _highlightRects.first().y == y ) {
        return;
    }

    OverlayRect rect;
    rect.x = x;
    rect.y = y;
    rect.width = 1;
    rect.height = 1;
    rect.color = QColor( 255, 255, 255, 70 );

    _highlightRects = { rect };

    update();
}

void Viewport::clearHighlight() {
    if ( _highlightRects.isEmpty() ) {
        return;
    }

    _highlightRects.clear();

    update();
}

void Viewport::setOverlayRects( const QVariantList& rects, const QColor& color ) {
    _overlayRects.clear();
    _overlayRects.reserve( rects.size() );

    for ( const QVariant& rectVariant : rects ) {
        const QVariantMap rectMap = rectVariant.toMap();

        OverlayRect rect;
        rect.x = rectMap.value( "x" ).toInt();
        rect.y = rectMap.value( "y" ).toInt();
        rect.width = rectMap.value( "width" ).toInt();
        rect.height = rectMap.value( "height" ).toInt();
        rect.color = color;

        _overlayRects.append( rect );
    }

    update();
}

void Viewport::clearOverlayRects() {
    if ( _overlayRects.isEmpty() ) {
        return;
    }

    _overlayRects.clear();

    update();
}

void Viewport::forceRedraw() {
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

    if ( x < 0 || y < 0 || x >= static_cast<int>( _world->width() ) || y >= static_cast<int>( _world->height() ) ) {
        return;
    }

    if ( event->button() == Qt::RightButton ) {
        emit tileRightClicked( x, y, z );
        return;
    }

    emit tileClicked( x, y, z );
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

QSGNode* Viewport::updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* ) {
    delete oldNode;

    auto* rootNode = new QSGNode();

    RenderScene scene;

    if ( !_world || width() <= 0.0 || height() <= 0.0 ) {
        return rootNode;
    }

    _renderer->render( scene, *_camera, *_world, _activeFloor );

    scene.build( rootNode, window(), *_camera, _textureCache );

    for ( const OverlayRect& rect : _highlightRects ) {
        addOverlayNode( rootNode, rect );
    }

    for ( const OverlayRect& rect : _overlayRects ) {
        addOverlayNode( rootNode, rect );
    }

    return rootNode;
}

void Viewport::addOverlayNode( QSGNode* parent, const OverlayRect& rect ) const {
    const double tileSize = WorldConstants::TILE_SIZE;

    const QPointF worldPosition( rect.x * tileSize, rect.y * tileSize );
    const QPointF screenPosition = _camera->worldToScreen( worldPosition );

    auto* node = new QSGSimpleRectNode();
    node->setColor( rect.color );
    node->setRect( screenPosition.x(), screenPosition.y(), rect.width * tileSize, rect.height * tileSize );

    parent->appendChildNode( node );
}

} // namespace Engine
