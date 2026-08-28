#include "RenderScene.h"

#include <QSGSimpleTextureNode>

namespace Engine {

RenderScene::RenderScene() {
}

RenderScene::~RenderScene() {
    clear();
}

void RenderScene::clear() {
    _items.clear();
}

void RenderScene::addTexture( const QPointF& position, const QSizeF& size, const QImage& image ) {
    RenderSceneItem item( position, size, image );
    _items.append( item );
}

void RenderScene::build( QSGNode* rootNode, QQuickWindow* window, const Camera& camera ) {
    if ( !rootNode || !window ) {
        return;
    }

    for ( const RenderSceneItem& item : _items ) {
        if ( item.image().isNull() ) {
            continue;
        }

        const QPointF screenPosition = camera.worldToScreen( item.position() );

        QSGTexture* texture = window->createTextureFromImage( item.image() );

        if ( !texture ) {
            continue;
        }

        auto* node = new QSGSimpleTextureNode();
        node->setTexture( texture );
        node->setOwnsTexture( true );
        node->setRect( screenPosition.x(), screenPosition.y(), item.size().width(), item.size().height() );

        rootNode->appendChildNode( node );
    }
}

} // namespace Engine
