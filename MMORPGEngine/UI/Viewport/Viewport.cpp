#include "Viewport.h"

#include <MMORPGEngine/Renderer/Camera/Camera.h>
#include <MMORPGEngine/Renderer/Renderer.h>

#include <QSGSimpleRectNode>

namespace Engine {

Viewport::Viewport( QQuickItem* parent ) :
    QQuickItem( parent ),
    _camera( new Camera() ),
    _renderer( new Renderer() ),
    _world( nullptr ) {

    setFlag( ItemHasContents, true );

    _renderer->initialize();
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

Camera* Viewport::camera() const {
    return _camera;
}

void Viewport::setRenderer( Renderer* renderer ) {
    _renderer = renderer;

    if ( _renderer ) {
        _renderer->initialize();
        _renderer->resize( size() );
    }

    update();
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

QSGNode* Viewport::updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* ) {
    delete oldNode;

    auto* rootNode = new QSGNode();

    RenderScene scene;

    if ( !_renderer || !_world ) {
        return rootNode;
    }

    _renderer->render( scene, *_camera, *_world );

    scene.build( rootNode, *_camera );

    return rootNode;
}

} // namespace Engine
