#ifndef OBJECTRENDERER_H
#define OBJECTRENDERER_H

#include <MMORPGEngine/Data/Object/ObjectTextureModel.h>
#include <MMORPGEngine/Object/ObjectModel.h>
#include <MMORPGEngine/Renderer/Camera/Camera.h>
#include <MMORPGEngine/Renderer/Scene/RenderScene.h>
#include <MMORPGEngine/Renderer/World/RenderWorld.h>

namespace Engine {

class ObjectRenderer {
public:
    ObjectRenderer();

    void render( RenderScene& scene, const Camera& camera, const RenderWorld& world );

private:
    void renderObject( RenderScene& scene, int x, int y, int z, const ObjectModel& object, const ObjectTextureModel& objectTexture );
};

} // namespace Engine

#endif // OBJECTRENDERER_H
