#include "ObjectRenderer.h"

#include <QColor>
#include <QDateTime>

#include <MMORPGEngine/Renderer/RenderVisibleArea.h>
#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

ObjectRenderer::ObjectRenderer() {
}

void ObjectRenderer::render( RenderScene& scene, const Camera& camera, const RenderWorld& world, int z ) {
    const RenderVisibleArea area( camera, world );

    if ( area.isEmpty() ) {
        return;
    }

    const qint64 elapsedMs = QDateTime::currentMSecsSinceEpoch();
    const std::vector<int> loadedFloors = world.floors();

    for ( int chunkY = area.firstChunkY(); chunkY <= area.lastChunkY(); ++chunkY ) {
        for ( int chunkX = area.firstChunkX(); chunkX <= area.lastChunkX(); ++chunkX ) {
            if ( !world.hasChunk( chunkX, chunkY ) ) {
                continue;
            }

            const QRect tiles = area.tilesInChunk( chunkX, chunkY );

            for ( int y = tiles.top(); y <= tiles.bottom(); ++y ) {
                for ( int x = tiles.left(); x <= tiles.right(); ++x ) {
                    const int effectiveZ = world.resolveFloor( x, y, z, loadedFloors );
                    const WorldObjectModel* object = world.object( x, y, effectiveZ );

                    if ( !object || !object->objectModel() ) {
                        continue;
                    }

                    renderObject( scene, x, y, effectiveZ, *object, elapsedMs );

                    if ( effectiveZ != z ) {
                        const QPointF position( x * WorldConstants::TILE_SIZE, y * WorldConstants::TILE_SIZE );
                        const QSizeF size( WorldConstants::TILE_SIZE, WorldConstants::TILE_SIZE );

                        scene.addRect( RenderSceneLayerEnum::Object, y, position, size, QColor( 0, 0, 0, 130 ) );
                    }
                }
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

    scene.addTexture( RenderSceneLayerEnum::Object, y, position, size, frame );
}

} // namespace Engine
