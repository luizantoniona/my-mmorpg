#include "TileRenderer.h"

#include <QColor>
#include <QDateTime>

#include <MMORPGEngine/Renderer/RenderVisibleArea.h>
#include <MMORPGEngine/World/WorldConstants.h>

namespace {

constexpr double LOW_DETAIL_ZOOM = 0.5;
constexpr int MAX_BAKES_PER_FRAME = 4;
constexpr quint64 EVICT_AFTER_FRAMES = 300;
constexpr int SEE_THROUGH_ALPHA = 130;

int tilePixels( int detail ) {
    if ( detail == 0 ) {
        return Engine::WorldConstants::TILE_SIZE;
    }

    return static_cast<int>( Engine::WorldConstants::TILE_SIZE * LOW_DETAIL_ZOOM / 2.0 );
}

} // namespace

namespace Engine {

TileRenderer::DynamicCell::DynamicCell( int x, int y, int z ) :
    x( x ),
    y( y ),
    z( z ) {
}

TileRenderer::ChunkEntry::ChunkEntry() :
    revision( 0 ),
    lastUsedFrame( 0 ),
    hasStaticTiles( false ) {
}

TileRenderer::TileRenderer() :
    _frame( 0 ) {
}

void TileRenderer::render( RenderScene& scene, const Camera& camera, const RenderWorld& world, int z ) {
    ++_frame;

    const RenderVisibleArea area( camera, world );

    if ( !area.isEmpty() ) {
        const int detail = camera.zoom() < LOW_DETAIL_ZOOM ? 1 : 0;
        const double chunkPixels = WorldConstants::CHUNK_SIZE * WorldConstants::TILE_SIZE;
        const std::vector<int> loadedFloors = world.floors();
        const qint64 elapsedMs = QDateTime::currentMSecsSinceEpoch();

        int bakes = 0;

        for ( int chunkY = area.firstChunkY(); chunkY <= area.lastChunkY(); ++chunkY ) {
            for ( int chunkX = area.firstChunkX(); chunkX <= area.lastChunkX(); ++chunkX ) {
                if ( !world.hasChunk( chunkX, chunkY ) ) {
                    continue;
                }

                ChunkEntry& entry = _entries[ entryKey( chunkX, chunkY, z, detail ) ];

                if ( isStale( entry, world.chunkRevision( chunkX, chunkY ) ) ) {
                    if ( bakes >= MAX_BAKES_PER_FRAME ) {
                        if ( entry.image.isNull() ) {
                            continue;
                        }
                    } else {
                        if ( !entry.image.isNull() ) {
                            scene.releaseTexture( entry.image.cacheKey() );
                        }

                        bake( entry, chunkX, chunkY, z, detail, world, loadedFloors, elapsedMs );

                        ++bakes;
                    }
                }

                entry.lastUsedFrame = _frame;

                if ( entry.hasStaticTiles ) {
                    scene.addTexture( RenderSceneLayerEnum::Tile, chunkY * WorldConstants::CHUNK_SIZE, QPointF( chunkX * chunkPixels, chunkY * chunkPixels ), QSizeF( chunkPixels, chunkPixels ), entry.image );
                }

                for ( const DynamicCell& cell : entry.dynamicCells ) {
                    const WorldTileModel* tile = world.tile( cell.x, cell.y, cell.z );

                    if ( !tile || !tile->tileModel() ) {
                        continue;
                    }

                    renderTile( scene, cell.x, cell.y, cell.z, *tile, elapsedMs );

                    if ( cell.z != z ) {
                        const QPointF position( cell.x * WorldConstants::TILE_SIZE, cell.y * WorldConstants::TILE_SIZE );
                        const QSizeF size( WorldConstants::TILE_SIZE, WorldConstants::TILE_SIZE );

                        scene.addRect( RenderSceneLayerEnum::Tile, cell.y, position, size, QColor( 0, 0, 0, SEE_THROUGH_ALPHA ) );
                    }
                }
            }
        }
    }

    evictUnused( scene );
}

quint64 TileRenderer::entryKey( int chunkX, int chunkY, int z, int detail ) {
    return ( static_cast<quint64>( chunkX ) << 40 ) | ( static_cast<quint64>( chunkY ) << 17 ) | ( static_cast<quint64>( static_cast<quint16>( z ) ) << 1 ) | static_cast<quint64>( detail );
}

bool TileRenderer::isStale( const ChunkEntry& entry, quint64 revision ) const {
    return entry.image.isNull() || entry.revision != revision;
}

void TileRenderer::bake( ChunkEntry& entry, int chunkX, int chunkY, int z, int detail, const RenderWorld& world, const std::vector<int>& loadedFloors, qint64 elapsedMs ) {
    const int chunkSize = WorldConstants::CHUNK_SIZE;
    const int pixels = tilePixels( detail );
    const int worldWidth = static_cast<int>( world.width() );
    const int worldHeight = static_cast<int>( world.height() );

    QImage image( chunkSize * pixels, chunkSize * pixels, QImage::Format_ARGB32_Premultiplied );
    image.fill( Qt::transparent );

    entry.revision = world.chunkRevision( chunkX, chunkY );
    entry.dynamicCells.clear();
    entry.hasStaticTiles = false;

    QPainter painter( &image );

    for ( int localY = 0; localY < chunkSize; ++localY ) {
        for ( int localX = 0; localX < chunkSize; ++localX ) {
            const int x = chunkX * chunkSize + localX;
            const int y = chunkY * chunkSize + localY;

            if ( x >= worldWidth || y >= worldHeight ) {
                continue;
            }

            const int effectiveZ = world.resolveFloor( x, y, z, loadedFloors );
            const WorldTileModel* tile = world.tile( x, y, effectiveZ );

            if ( !tile || !tile->tileModel() ) {
                continue;
            }

            const bool animated = detail == 0 && ( tile->tileModel()->animation().isAnimated() || ( tile->overlayModel() && tile->overlayModel()->animation().isAnimated() ) );

            if ( animated ) {
                entry.dynamicCells.append( DynamicCell( x, y, effectiveZ ) );
                continue;
            }

            const QRect target( localX * pixels, localY * pixels, pixels, pixels );

            drawFrame( painter, target, tile->tileModel()->animation().frameAt( elapsedMs ), detail );

            if ( tile->overlayModel() ) {
                drawFrame( painter, target, tile->overlayModel()->animation().frameAt( elapsedMs ), detail );
            }

            if ( effectiveZ != z ) {
                painter.fillRect( target, QColor( 0, 0, 0, SEE_THROUGH_ALPHA ) );
            }

            entry.hasStaticTiles = true;
        }
    }

    painter.end();

    entry.image = image;
}

void TileRenderer::drawFrame( QPainter& painter, const QRect& target, const QImage& frame, int detail ) {
    if ( frame.isNull() ) {
        return;
    }

    if ( detail == 0 ) {
        painter.drawImage( target, frame );
        return;
    }

    auto it = _scaledFrames.find( frame.cacheKey() );

    if ( it == _scaledFrames.end() ) {
        it = _scaledFrames.insert( frame.cacheKey(), frame.scaled( target.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation ) );
    }

    painter.drawImage( target.topLeft(), it.value() );
}

void TileRenderer::renderTile( RenderScene& scene, int x, int y, int z, const WorldTileModel& worldTile, qint64 elapsedMs ) {
    Q_UNUSED( z );

    const QPointF position( x * WorldConstants::TILE_SIZE, y * WorldConstants::TILE_SIZE );
    const QSizeF size( WorldConstants::TILE_SIZE, WorldConstants::TILE_SIZE );

    const QImage frame = worldTile.tileModel()->animation().frameAt( elapsedMs );
    if ( !frame.isNull() ) {
        scene.addTexture( RenderSceneLayerEnum::Tile, y, position, size, frame );
    }

    if ( !worldTile.overlayModel() ) {
        return;
    }

    const QImage overlayFrame = worldTile.overlayModel()->animation().frameAt( elapsedMs );
    if ( !overlayFrame.isNull() ) {
        scene.addTexture( RenderSceneLayerEnum::Tile, y, position, size, overlayFrame );
    }
}

void TileRenderer::evictUnused( RenderScene& scene ) {
    for ( auto it = _entries.begin(); it != _entries.end(); ) {
        if ( _frame - it->lastUsedFrame <= EVICT_AFTER_FRAMES ) {
            ++it;
            continue;
        }

        if ( !it->image.isNull() ) {
            scene.releaseTexture( it->image.cacheKey() );
        }

        it = _entries.erase( it );
    }
}

} // namespace Engine
