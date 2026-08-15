#include "Renderer.h"

namespace Engine {

Renderer::Renderer() :
    _viewportSize( 0.0, 0.0 ),
    _tileRenderer( new TileRenderer() ) {
}

void Renderer::initialize() {
}

void Renderer::resize( const QSizeF& size ) {
    _viewportSize = size;
}

void Renderer::render( RenderScene& scene, const Camera& camera, const RenderWorld& world ) {
    _tileRenderer->render( scene, camera, world );
}

} // namespace Engine
