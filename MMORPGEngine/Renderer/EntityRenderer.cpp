#include "EntityRenderer.h"

#include <QColor>
#include <QPainter>

#include <MMORPGEngine/World/WorldConstants.h>

namespace {
constexpr int ENTITY_CULLING_MARGIN_TILES = 1;
constexpr int DIMMED_ALPHA = 130;
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

    const auto isCulled = [ startX, startY, endX, endY ]( const RenderWorld::Entity& entity ) {
        return entity.x + entity.footprint < startX || entity.x > endX || entity.y + entity.footprint < startY || entity.y > endY;
    };

    for ( const RenderWorld::Entity& entity : world.entities( z ) ) {
        if ( !isCulled( entity ) ) {
            renderEntity( scene, entity, false );
        }
    }

    const std::vector<int> loadedFloors = world.floors();

    for ( const RenderWorld::Entity& entity : world.entities( z - 1 ) ) {
        if ( !isCulled( entity ) && world.resolveFloor( entity.x, entity.y, z, loadedFloors ) != z ) {
            renderEntity( scene, entity, true );
        }
    }
}

void EntityRenderer::renderEntity( RenderScene& scene, const RenderWorld::Entity& entity, bool dimmed ) {
    if ( entity.texture.isNull() ) {
        return;
    }

    const QImage& texture = dimmed ? dimmedTexture( entity.texture ) : entity.texture;

    const double footprintWidth = entity.footprint * WorldConstants::TILE_SIZE;
    const QPointF position( entity.x * WorldConstants::TILE_SIZE + ( footprintWidth - texture.width() ) / 2.0 + entity.offsetX, ( entity.y + entity.footprint ) * WorldConstants::TILE_SIZE - texture.height() + entity.offsetY );
    const QSizeF size( texture.width(), texture.height() );

    const int row = static_cast<int>( std::floor( ( entity.y * WorldConstants::TILE_SIZE + entity.offsetY ) / WorldConstants::TILE_SIZE ) ) + entity.footprint - 1;

    scene.addTexture( RenderSceneLayerEnum::Entity, row, position, size, texture );
}

const QImage& EntityRenderer::dimmedTexture( const QImage& texture ) {
    const qint64 key = texture.cacheKey();

    auto it = _dimmedTextures.find( key );
    if ( it != _dimmedTextures.end() ) {
        return it.value();
    }

    QImage dimmed = texture.convertToFormat( QImage::Format_ARGB32_Premultiplied );

    QPainter painter( &dimmed );
    painter.setCompositionMode( QPainter::CompositionMode_SourceAtop );
    painter.fillRect( dimmed.rect(), QColor( 0, 0, 0, DIMMED_ALPHA ) );
    painter.end();

    return _dimmedTextures.insert( key, dimmed ).value();
}

} // namespace Engine
