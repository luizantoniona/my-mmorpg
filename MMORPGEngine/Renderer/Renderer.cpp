#include "Renderer.h"

namespace Engine {

Renderer::Renderer() :
    _viewportSize( 0.0, 0.0 ),
    _effectRenderer( new EffectRenderer() ),
    _entityRenderer( new EntityRenderer() ),
    _objectRenderer( new ObjectRenderer() ),
    _overlayRenderer( new OverlayRenderer() ),
    _tileRenderer( new TileRenderer() ) {
}

void Renderer::initialize() {
}

void Renderer::resize( const QSizeF& size ) {
    _viewportSize = size;
}

OverlayRenderer* Renderer::overlayRenderer() const {
    return _overlayRenderer;
}

EffectRenderer* Renderer::effectRenderer() const {
    return _effectRenderer;
}

void Renderer::render( RenderScene& scene, const Camera& camera, const RenderWorld& world, int z ) {
    _tileRenderer->render( scene, camera, world, z );
    _objectRenderer->render( scene, camera, world, z );
    _entityRenderer->render( scene, camera, world, z );
    _overlayRenderer->render( scene );
    _effectRenderer->render( scene );
}

} // namespace Engine
