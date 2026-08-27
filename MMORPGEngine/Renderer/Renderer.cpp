#include "Renderer.h"

namespace Engine {

Renderer::Renderer() :
    _viewportSize( 0.0, 0.0 ),
    _objectRenderer( new ObjectRenderer() ),
    _tileRenderer( new TileRenderer() ) {
}

void Renderer::initialize() {
}

void Renderer::resize( const QSizeF& size ) {
    _viewportSize = size;
}

void Renderer::render( RenderScene& scene, const Camera& camera, const RenderWorld& world ) {
    // TODO: Order is important here, see if we can structure it in a better way
    _tileRenderer->render( scene, camera, world );
    _objectRenderer->render( scene, camera, world );
}

} // namespace Engine
