#ifndef RENDERER_H
#define RENDERER_H

#include <QSizeF>

#include <MMORPGEngine/Renderer/ObjectRenderer.h>
#include <MMORPGEngine/Renderer/Scene/RenderScene.h>
#include <MMORPGEngine/Renderer/TileRenderer.h>

namespace Engine {

class Renderer {
public:
    Renderer();

    void initialize();

    void resize( const QSizeF& size );

    void render( RenderScene& scene, const Camera& camera, const RenderWorld& world );

private:
    QSizeF _viewportSize;
    ObjectRenderer* _objectRenderer;
    TileRenderer* _tileRenderer;
};

} // namespace Engine

#endif // RENDERER_H
