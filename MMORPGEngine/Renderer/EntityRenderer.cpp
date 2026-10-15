#include "EntityRenderer.h"

#include <MMORPGEngine/World/WorldConstants.h>

namespace {
constexpr int ENTITY_CULLING_MARGIN_TILES = 1;
} // namespace

namespace Engine {

EntityRenderer::EntityRenderer() {
}

void EntityRenderer::render( RenderScene& scene, const Camera& camera, const RenderWorld& world, int z ) {
    const QRectF visibleRect = camera.visibleRect();

    const int startX = static_cast<int>( std::floor( visibleRect.left() / WorldConstants::TILE_SIZE ) ) - ENTITY_CULLING_MARGIN_TILES;
    const int startY = static_cast<int>( std::floor( visibleRect.top() / WorldConstants::TILE_SIZE ) ) - ENTITY_CULLING_MARGIN_TILES;
    const int endX = static_cast<int>( std::ceil( visibleRect.right() / WorldConstants::TILE_SIZE ) ) + ENTITY_CULLING_MARGIN_TILES;
    const int endY = static_cast<int>( std::ceil( visibleRect.bottom() / WorldConstants::TILE_SIZE ) ) + ENTITY_CULLING_MARGIN_TILES;

    for ( const RenderWorld::Entity& entity : world.entities( z ) ) {
        if ( entity.x + entity.footprint < startX || entity.x > endX || entity.y + entity.footprint < startY || entity.y > endY ) {
            continue;
        }

        if ( entity.texture.isNull() ) {
            continue;
        }

        const double footprintWidth = entity.footprint * WorldConstants::TILE_SIZE;
        const QPointF position( entity.x * WorldConstants::TILE_SIZE + ( footprintWidth - entity.texture.width() ) / 2.0 + entity.offsetX, ( entity.y + entity.footprint ) * WorldConstants::TILE_SIZE - entity.texture.height() + entity.offsetY );
        const QSizeF size( entity.texture.width(), entity.texture.height() );

        const int row = static_cast<int>( std::floor( ( entity.y * WorldConstants::TILE_SIZE + entity.offsetY ) / WorldConstants::TILE_SIZE ) ) + entity.footprint - 1;

        scene.addTexture( RenderSceneLayerEnum::Entity, row, position, size, entity.texture );
    }
}

} // namespace Engine
