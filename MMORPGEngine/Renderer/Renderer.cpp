#include "Renderer.h"

#include <QSGSimpleRectNode>

namespace Engine {

Renderer::Renderer() :
    _viewportSize( 0.0, 0.0 ) {
}

void Renderer::initialize() {
}

void Renderer::resize( const QSizeF& size ) {
    _viewportSize = size;
}

void Renderer::render( QSGNode* rootNode ) {
    auto* node = new QSGSimpleRectNode();

    node->setRect( 100.0, 100.0, 200.0, 200.0 );

    node->setColor( Qt::red );

    rootNode->appendChildNode( node );
}

} // namespace Engine
