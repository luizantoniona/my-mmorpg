#include "ObjectRenderer.h"

#include <QColor>
#include <QDateTime>

#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

ObjectRenderer::ObjectRenderer() {
}

void ObjectRenderer::render( RenderScene& scene, const Camera& camera, const RenderWorld& world, int z ) {
    const QRectF visibleRect = camera.visibleRect();

    const int startX = static_cast<int>( std::floor( visibleRect.left() / WorldConstants::TILE_SIZE ) );
    const int startY = static_cast<int>( std::floor( visibleRect.top() / WorldConstants::TILE_SIZE ) );
    const int endX = static_cast<int>( std::ceil( visibleRect.right() / WorldConstants::TILE_SIZE ) );
    const int endY = static_cast<int>( std::ceil( visibleRect.bottom() / WorldConstants::TILE_SIZE ) );

    const qint64 elapsedMs = QDateTime::currentMSecsSinceEpoch();
    const std::vector<int> loadedFloors = world.floors();

    for ( int y = startY; y <= endY; ++y ) {
        for ( int x = startX; x <= endX; ++x ) {
            const int effectiveZ = world.resolveFloor( x, y, z, loadedFloors );
            const WorldObjectModel* object = world.object( x, y, effectiveZ );

            if ( !object || !object->objectModel() ) {
                continue;
            }

            renderObject( scene, x, y, effectiveZ, *object, elapsedMs );

            if ( effectiveZ != z ) {
                const QPointF position( x * WorldConstants::TILE_SIZE, y * WorldConstants::TILE_SIZE );
                const QSizeF size( WorldConstants::TILE_SIZE, WorldConstants::TILE_SIZE );

                scene.addRect( position, size, QColor( 0, 0, 0, 130 ) );
            }
        }
    }
}

void ObjectRenderer::renderObject( RenderScene& scene, int x, int y, int z, const WorldObjectModel& worldObject, qint64 elapsedMs ) {
    Q_UNUSED( z );

    const ObjectModel* objectModel = worldObject.objectModel();
    if ( !objectModel ) {
        return;
    }

    const QImage frame = objectModel->animation().frameAt( elapsedMs );
    if ( frame.isNull() ) {
        return;
    }

    const ObjectSizeModel footprint = objectModel->size();
    const qreal scale = ( footprint.width() * WorldConstants::TILE_SIZE ) / static_cast<qreal>( frame.width() );
    const QSizeF size( frame.width() * scale, frame.height() * scale );
    const QPointF position( x * WorldConstants::TILE_SIZE, ( y + 1 ) * WorldConstants::TILE_SIZE - size.height() );

    scene.addTexture( position, size, frame );
}

} // namespace Engine
