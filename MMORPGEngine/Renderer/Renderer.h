#ifndef RENDERER_H
#define RENDERER_H

#include <QSizeF>

#include <MMORPGEngine/Renderer/RenderScene.h>

namespace Engine {

class Renderer {
public:
    Renderer();

    void initialize();

    void resize( const QSizeF& size );

    void render( RenderScene& scene, const Camera& camera );

private:
    QSizeF _viewportSize;
};

} // namespace Engine

#endif // RENDERER_H
