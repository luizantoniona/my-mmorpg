#ifndef RENDERER_H
#define RENDERER_H

#include <QImage>
#include <QSizeF>

#include <MMORPGEngine/Renderer/EntityRenderer.h>
#include <MMORPGEngine/Renderer/ObjectRenderer.h>
#include <MMORPGEngine/Renderer/Scene/RenderScene.h>
#include <MMORPGEngine/Renderer/TileRenderer.h>

namespace Engine {

class Renderer {
public:
    Renderer();

    void initialize();

    void resize( const QSizeF& size );

    void render( RenderScene& scene, const Camera& camera, const RenderWorld& world, int z );

private:
    QSizeF _viewportSize;
    QImage _entityTexture;
    EntityRenderer* _entityRenderer;
    ObjectRenderer* _objectRenderer;
    TileRenderer* _tileRenderer;
};

} // namespace Engine

#endif // RENDERER_H
