#ifndef EFFECTRENDERER_H
#define EFFECTRENDERER_H

#include <QColor>
#include <QElapsedTimer>
#include <QList>

#include <MMORPGEngine/Renderer/Scene/RenderScene.h>

namespace Engine {

class EffectRenderer {
public:
    EffectRenderer();

    void addTileFlash( int x, int y, const QColor& color, int durationMs );
    void addTileWarning( int x, int y, const QColor& color, int durationMs );

    void render( RenderScene& scene );

private:
    class TileFlash {
    public:
        QColor color;
        qint64 startMs;
        int x;
        int y;
        int durationMs;
        bool intensifying;
    };

    void appendTileFlash( int x, int y, const QColor& color, int durationMs, bool intensifying );
    void renderTileFlash( RenderScene& scene, const TileFlash& flash, double progress ) const;

private:
    QList<TileFlash> _tileFlashes;
    QElapsedTimer _clock;
};

} // namespace Engine

#endif // EFFECTRENDERER_H
