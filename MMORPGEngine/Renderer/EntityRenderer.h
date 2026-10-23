#ifndef ENTITYRENDERER_H
#define ENTITYRENDERER_H

#include <QHash>
#include <QImage>

#include <MMORPGEngine/Renderer/Camera/Camera.h>
#include <MMORPGEngine/Renderer/Scene/RenderScene.h>
#include <MMORPGEngine/Renderer/World/RenderWorld.h>

namespace Engine {

class EntityRenderer {
public:
    EntityRenderer();

    void render( RenderScene& scene, const Camera& camera, const RenderWorld& world, int z );

private:
    void renderEntity( RenderScene& scene, const RenderWorld::Entity& entity, bool dimmed );

    const QImage& dimmedTexture( const QImage& texture );

private:
    QHash<qint64, QImage> _dimmedTextures;
};

} // namespace Engine

#endif // ENTITYRENDERER_H
