#include "ObjectRenderer.h"

#include <QColor>

namespace {
constexpr int TILE_SIZE = 32;
} // namespace

namespace Engine {

ObjectRenderer::ObjectRenderer() {
}

void ObjectRenderer::render( RenderScene& scene, const Camera& camera, const RenderWorld& world ) {
    const QRectF visibleRect = camera.visibleRect();

    const int startX = static_cast<int>( std::floor( visibleRect.left() / TILE_SIZE ) );

    const int startY = static_cast<int>( std::floor( visibleRect.top() / TILE_SIZE ) );

    const int endX = static_cast<int>( std::ceil( visibleRect.right() / TILE_SIZE ) );

    const int endY = static_cast<int>( std::ceil( visibleRect.bottom() / TILE_SIZE ) );

    constexpr int z = 0;

    for ( int y = startY; y <= endY; ++y ) {
        for ( int x = startX; x <= endX; ++x ) {
            const ObjectModel* object = world.object( x, y, z );

            if ( !object ) {
                continue;
            }

            const ObjectTextureModel* objectTexture = world.objectTexture( object->objectTextureId() );

            if ( !objectTexture ) {
                continue;
            }

            renderObject( scene, x, y, z, *object, *objectTexture );
        }
    }
}

void ObjectRenderer::renderObject( RenderScene& scene, int x, int y, int z, const ObjectModel& object, const ObjectTextureModel& objectTexture ) {
    Q_UNUSED( z );

    const QPointF position( x * TILE_SIZE, y * TILE_SIZE );

    const QSizeF size( TILE_SIZE, TILE_SIZE );

    if ( objectTexture.texture().isNull() ) {
        return;
    }

    scene.addTexture( position, size, objectTexture.texture() );
}

} // namespace Engine
