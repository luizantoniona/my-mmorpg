#ifndef TILERENDERER_H
#define TILERENDERER_H

#include <MMORPGEngine/Renderer/RenderScene.h>

namespace Engine {

class TileRenderer {
public:
    TileRenderer();

    void render( RenderScene& scene, const Camera& camera );

private:
    void renderTile( RenderScene& scene, int x, int y );
};

} // namespace Engine

#endif // TILERENDERER_H
