#ifndef RENDERER_H
#define RENDERER_H

#include <QSizeF>

#include <MMORPGEngine/Renderer/EffectRenderer.h>
#include <MMORPGEngine/Renderer/EntityRenderer.h>
#include <MMORPGEngine/Renderer/ObjectRenderer.h>
#include <MMORPGEngine/Renderer/OverlayRenderer.h>
#include <MMORPGEngine/Renderer/Scene/RenderScene.h>
#include <MMORPGEngine/Renderer/TileRenderer.h>

namespace Engine {

class Renderer {
public:
    Renderer();

    void initialize();

    void resize( const QSizeF& size );

    OverlayRenderer* overlayRenderer() const;
    EffectRenderer* effectRenderer() const;

    void render( RenderScene& scene, const Camera& camera, const RenderWorld& world, int z );

private:
    QSizeF _viewportSize;
    EffectRenderer* _effectRenderer;
    EntityRenderer* _entityRenderer;
    ObjectRenderer* _objectRenderer;
    OverlayRenderer* _overlayRenderer;
    TileRenderer* _tileRenderer;
};

} // namespace Engine

#endif // RENDERER_H
