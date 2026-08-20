#ifndef RENDERSCENE_H
#define RENDERSCENE_H

#include <QImage>
#include <QList>
#include <QQuickWindow>
#include <QSGNode>

#include <MMORPGEngine/Renderer/Camera/Camera.h>
#include <MMORPGEngine/Renderer/Scene/RenderSceneItem.h>

namespace Engine {

class RenderScene {
public:
    RenderScene();
    ~RenderScene();

    void clear();

    void addTexture( const QPointF& position, const QSizeF& size, const QImage& image );

    void build( QSGNode* rootNode, QQuickWindow* window, const Camera& camera );

private:
    QList<RenderSceneItem> _items;
};

} // namespace Engine

#endif // RENDERSCENE_H
