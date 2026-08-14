#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QQuickItem>

namespace Engine {

class Viewport : public QQuickItem {
    Q_OBJECT

public:
    explicit Viewport( QQuickItem* parent = nullptr );

protected:
    QSGNode* updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData ) override;
};

} // namespace Engine

#endif // VIEWPORT_H
