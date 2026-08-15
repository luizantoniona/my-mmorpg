#ifndef TILERENDERER_H
#define TILERENDERER_H

#include <MMORPGEngine/Renderer/Camera/Camera.h>
#include <MMORPGEngine/Renderer/Scene/RenderScene.h>
#include <MMORPGEngine/Renderer/World/RenderWorld.h>

namespace Engine {

class TileRenderer {
public:
    TileRenderer();

    void render( RenderScene& scene, const Camera& camera, const RenderWorld& world );

private:
    void renderTile( RenderScene& scene, int x, int y, int z );
};

} // namespace Engine

#endif // TILERENDERER_H
