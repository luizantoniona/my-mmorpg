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
    _world( nullptr ),
    _activeFloor( 0 ) {

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
    const int z = _activeFloor;

    if ( !_world->tile( x, y, z ) ) {
        return;
    }

    emit tileClicked( x, y, z );
}

QSGNode* Viewport::updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* ) {
    delete oldNode;

    auto* rootNode = new QSGNode();

    RenderScene scene;

    // Sem tamanho definido ainda, a câmera não tem como clampar a
    // posição corretamente; evita desenhar um frame descentralizado
    // antes do primeiro geometryChange real.
    if ( !_world || width() <= 0.0 || height() <= 0.0 ) {
        return rootNode;
    }

    _renderer->render( scene, *_camera, *_world, _activeFloor );

    scene.build( rootNode, window(), *_camera );

    return rootNode;
}

} // namespace Engine
