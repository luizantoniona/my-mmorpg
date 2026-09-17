#include "RenderScene.h"

#include <QSGSimpleRectNode>
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

void RenderScene::addRect( const QPointF& position, const QSizeF& size, const QColor& color ) {
    RenderSceneItem item( position, size, color );
    _items.append( item );
}

void RenderScene::build( QSGNode* rootNode, QQuickWindow* window, const Camera& camera, TextureCache& textureCache ) {
    if ( !rootNode || !window ) {
        return;
    }

    for ( const RenderSceneItem& item : _items ) {
        const QPointF screenPosition = camera.worldToScreen( item.position() );

        if ( item.color().isValid() ) {
            auto* rectNode = new QSGSimpleRectNode();
            rectNode->setColor( item.color() );
            rectNode->setRect( screenPosition.x(), screenPosition.y(), item.size().width(), item.size().height() );

            rootNode->appendChildNode( rectNode );
            continue;
        }

        if ( item.image().isNull() ) {
            continue;
        }

        QSGTexture* texture = textureCache.texture( window, item.image() );

        if ( !texture ) {
            continue;
        }

        auto* node = new QSGSimpleTextureNode();
        node->setTexture( texture );
        node->setOwnsTexture( false );
        node->setRect( screenPosition.x(), screenPosition.y(), item.size().width(), item.size().height() );

        rootNode->appendChildNode( node );
    }
}

} // namespace Engine
