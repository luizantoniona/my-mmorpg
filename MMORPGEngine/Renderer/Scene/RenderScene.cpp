#include "RenderScene.h"

#include <QSGNode>
#include <QSGSimpleRectNode>

namespace Engine {

RenderScene::RenderScene() {
}

RenderScene::~RenderScene() {
    clear();
}

void RenderScene::clear() {
    _rects.clear();
}

void RenderScene::addRect( const QPointF& position, const QSizeF& size, const QColor& color ) {
    Rect rect;

    rect.position = position;
    rect.size = size;
    rect.color = color;

    _rects.append( rect );
}

void RenderScene::build( QSGNode* rootNode, const Camera& camera ) {
    if ( !rootNode ) {
        return;
    }

    for ( const Rect& rect : _rects ) {
        const QPointF screenPosition = camera.worldToScreen( rect.position );

        auto* node = new QSGSimpleRectNode();

        node->setRect( screenPosition.x(), screenPosition.y(),
                       rect.size.width(), rect.size.height() );
        node->setColor( rect.color );

        rootNode->appendChildNode( node );
    }
}

} // namespace Engine
