#ifndef ENTITYRENDERER_H
#define ENTITYRENDERER_H

#include <QImage>

#include <MMORPGEngine/Renderer/Camera/Camera.h>
#include <MMORPGEngine/Renderer/Scene/RenderScene.h>
#include <MMORPGEngine/Renderer/World/RenderWorld.h>

namespace Engine {

class EntityRenderer {
public:
    EntityRenderer();

    void render( RenderScene& scene, const Camera& camera, const RenderWorld& world, int z );
};

} // namespace Engine

#endif // ENTITYRENDERER_H
