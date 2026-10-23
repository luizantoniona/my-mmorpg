#ifndef TILERENDERER_H
#define TILERENDERER_H

#include <vector>

#include <QHash>
#include <QImage>
#include <QList>
#include <QPainter>

#include <MMORPGEngine/Renderer/Camera/Camera.h>
#include <MMORPGEngine/Renderer/Scene/RenderScene.h>
#include <MMORPGEngine/Renderer/World/RenderWorld.h>
#include <MMORPGEngine/World/Tile/WorldTileModel.h>

namespace Engine {

class TileRenderer {
public:
    TileRenderer();

    void render( RenderScene& scene, const Camera& camera, const RenderWorld& world, int z );

private:
    class DynamicCell {
    public:
        DynamicCell( int x, int y, int z );

        int x;
        int y;
        int z;
    };

    class ChunkEntry {
    public:
        ChunkEntry();

        QImage image;
        QList<DynamicCell> dynamicCells;
        quint64 revision;
        quint64 lastUsedFrame;
        bool hasStaticTiles;
    };

    static quint64 entryKey( int chunkX, int chunkY, int z, int detail );

    bool isStale( const ChunkEntry& entry, quint64 revision ) const;

    void bake( ChunkEntry& entry, int chunkX, int chunkY, int z, int detail, const RenderWorld& world, const std::vector<int>& loadedFloors, qint64 elapsedMs );
    void drawFrame( QPainter& painter, const QRect& target, const QImage& frame, int detail );

    void renderTile( RenderScene& scene, int x, int y, int z, const WorldTileModel& worldTile, qint64 elapsedMs );
    void evictUnused( RenderScene& scene );

private:
    QHash<quint64, ChunkEntry> _entries;
    QHash<qint64, QImage> _scaledFrames;
    quint64 _frame;
};

} // namespace Engine

#endif // TILERENDERER_H
