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

void Renderer::render( RenderScene& scene, const Camera& ) {
    auto* node = new QSGSimpleRectNode();

    scene.addRect(
        QPointF( 0.0, 0.0 ),
        QSizeF( 100.0, 100.0 ) );
}

} // namespace Engine
