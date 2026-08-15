#ifndef RENDERER_H
#define RENDERER_H

#include <QSGNode>
#include <QSizeF>

namespace Engine {

class Renderer {
public:
    Renderer();

    void initialize();

    void resize( const QSizeF& size );

    void render( QSGNode* rootNode );

private:
    QSizeF _viewportSize;
};

} // namespace Engine

#endif // RENDERER_H
