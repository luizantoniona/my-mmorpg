#include "Viewport.h"

#include <MMORPGEngine/Renderer/Camera.h>
#include <MMORPGEngine/Renderer/Renderer.h>

#include <QSGSimpleRectNode>

namespace Engine {

Viewport::Viewport( QQuickItem* parent ) :
    QQuickItem( parent ),
    _renderer( nullptr ) {

    setFlag( ItemHasContents, true );
}

void Viewport::setRenderer( Renderer* renderer ) {
    _renderer = renderer;

    if ( _renderer ) {
        _renderer->initialize();
        _renderer->resize( size() );
    }

    update();
}

QSGNode* Viewport::updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* ) {
    if ( oldNode ) {
        delete oldNode;
    }

    auto* rootNode = new QSGNode();

    if ( _renderer ) {
        _renderer->render( rootNode );
    }

    return rootNode;
}

} // namespace Engine
