#include "ObjectRenderer.h"

#include <QColor>

#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

ObjectRenderer::ObjectRenderer() {
}

void ObjectRenderer::render( RenderScene& scene, const Camera& camera, const RenderWorld& world ) {
    const QRectF visibleRect = camera.visibleRect();

    const int startX = static_cast<int>( std::floor( visibleRect.left() / WorldConstants::TILE_SIZE ) );
    const int startY = static_cast<int>( std::floor( visibleRect.top() / WorldConstants::TILE_SIZE ) );
    const int endX = static_cast<int>( std::ceil( visibleRect.right() / WorldConstants::TILE_SIZE ) );
    const int endY = static_cast<int>( std::ceil( visibleRect.bottom() / WorldConstants::TILE_SIZE ) );

    constexpr int z = 0;

    for ( int y = startY; y <= endY; ++y ) {
        for ( int x = startX; x <= endX; ++x ) {
            const WorldObjectModel* object = world.object( x, y, z );

            if ( !object || !object->objectModel() ) {
                continue;
            }

            renderObject( scene, x, y, z, *object );
        }
    }
}

void ObjectRenderer::renderObject( RenderScene& scene, int x, int y, int z, const WorldObjectModel& worldObject ) {
    Q_UNUSED( z );

    const ObjectModel* objectModel = worldObject.objectModel();
    if ( !objectModel ) {
        return;
    }

    const QImage& texture = objectModel->texture();
    if ( texture.isNull() ) {
        return;
    }

    const QPointF position( x * WorldConstants::TILE_SIZE, y * WorldConstants::TILE_SIZE );
    const QSizeF size( texture.width(), texture.height() );

    scene.addTexture( position, size, texture );
}

} // namespace Engine
