#include "Viewport.h"

#include <MMORPGEngine/Renderer/Camera/Camera.h>
#include <MMORPGEngine/Renderer/Renderer.h>
#include <MMORPGEngine/World/WorldConstants.h>

#include <QSGSimpleRectNode>

namespace Engine {

Viewport::Viewport( QQuickItem* parent ) :
    QQuickItem( parent ),
    _camera( new Camera() ),
    _renderer( new Renderer() ),
    _world( nullptr ) {

    setFlag( ItemHasContents, true );

    setAcceptedMouseButtons( Qt::LeftButton );

    _renderer->initialize();
    _renderer->resize( size() );

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

    _world = world;

    update();
}

void Viewport::geometryChange( const QRectF& newGeometry, const QRectF& oldGeometry ) {
    QQuickItem::geometryChange( newGeometry, oldGeometry );
    _camera->setViewportSize( newGeometry.size() );
    _renderer->resize( newGeometry.size() );
}

void Viewport::mousePressEvent( QMouseEvent* event ) {
    if ( !_world ) {
        return;
    }

    // TODO: RightButton and WheelButton
    if ( event->button() != Qt::LeftButton ) {
        QQuickItem::mousePressEvent( event );
        return;
    }

    const QPointF screenPosition = event->position();
    const QPointF worldPosition = _camera->screenToWorld( screenPosition );

    const double tileSize = WorldConstants::TILE_SIZE;

    const int x = static_cast<int>( std::floor( worldPosition.x() / tileSize ) );
    const int y = static_cast<int>( std::floor( worldPosition.y() / tileSize ) );
    // TODO: Z
    constexpr int z = 0;

    if ( !_world->tile( x, y, z ) ) {
        return;
    }

    emit tileClicked( x, y, z );
}

QSGNode* Viewport::updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* ) {
    delete oldNode;

    auto* rootNode = new QSGNode();

    RenderScene scene;

    if ( !_world ) {
        return rootNode;
    }

    _renderer->render( scene, *_camera, *_world );

    scene.build( rootNode, window(), *_camera );

    return rootNode;
}

} // namespace Engine
