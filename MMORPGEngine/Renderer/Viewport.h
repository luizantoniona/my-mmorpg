#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QQuickItem>

#include <MMORPGEngine/Renderer/Renderer.h>

namespace Engine {

class Viewport : public QQuickItem {
    Q_OBJECT

public:
    explicit Viewport( QQuickItem* parent = nullptr );

    void setRenderer( Renderer* renderer );

protected:
    QSGNode* updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData ) override;

private:
    Renderer* _renderer;
};

} // namespace Engine

#endif // VIEWPORT_H
