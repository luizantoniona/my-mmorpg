#ifndef RENDERSCENE_H
#define RENDERSCENE_H

#include <QColor>
#include <QList>

#include <MMORPGEngine/Renderer/Camera/Camera.h>

class QSGNode;

namespace Engine {

class RenderScene {
public:
    RenderScene();
    ~RenderScene();

    void clear();

    void addRect( const QPointF& position, const QSizeF& size, const QColor& color );

    void build( QSGNode* rootNode, const Camera& camera );

private:
    class Rect {
    public:
        QPointF position;
        QSizeF size;
        QColor color;
    };

    QList<Rect> _rects;
};

} // namespace Engine

#endif // RENDERSCENE_H
