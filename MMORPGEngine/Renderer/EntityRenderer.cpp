#include "EntityRenderer.h"

#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

EntityRenderer::EntityRenderer() {
}

void EntityRenderer::render( RenderScene& scene, const Camera& camera, const RenderWorld& world, int z ) {
    const QRectF visibleRect = camera.visibleRect();

    const int startX = static_cast<int>( std::floor( visibleRect.left() / WorldConstants::TILE_SIZE ) );
    const int startY = static_cast<int>( std::floor( visibleRect.top() / WorldConstants::TILE_SIZE ) );
    const int endX = static_cast<int>( std::ceil( visibleRect.right() / WorldConstants::TILE_SIZE ) );
    const int endY = static_cast<int>( std::ceil( visibleRect.bottom() / WorldConstants::TILE_SIZE ) );

    for ( const RenderWorld::Entity& entity : world.entities( z ) ) {
        if ( entity.x < startX || entity.x > endX || entity.y < startY || entity.y > endY ) {
            continue;
        }

        if ( entity.texture.isNull() ) {
            continue;
        }

        const QPointF position( entity.x * WorldConstants::TILE_SIZE, ( entity.y + 1 ) * WorldConstants::TILE_SIZE - entity.texture.height() );
        const QSizeF size( entity.texture.width(), entity.texture.height() );

        scene.addTexture( position, size, entity.texture );
    }
}

} // namespace Engine
