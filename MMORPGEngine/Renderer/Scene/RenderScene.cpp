#include "RenderScene.h"

#include <algorithm>

#include <QSGSimpleRectNode>
#include <QSGSimpleTextureNode>

namespace {

int sortBand( Engine::RenderSceneLayerEnum layer ) {
    switch ( layer ) {
    case Engine::RenderSceneLayerEnum::Tile:
        return 0;
    case Engine::RenderSceneLayerEnum::Object:
    case Engine::RenderSceneLayerEnum::Entity:
        return 1;
    case Engine::RenderSceneLayerEnum::Overlay:
        return 2;
    case Engine::RenderSceneLayerEnum::Effect:
        return 3;
    }

    return 0;
}

} // namespace

namespace Engine {

RenderScene::RenderScene() {
}

RenderScene::~RenderScene() {
    clear();
}

void RenderScene::clear() {
    _items.clear();
}

void RenderScene::addTexture( RenderSceneLayerEnum layer, int row, const QPointF& position, const QSizeF& size, const QImage& image ) {
    RenderSceneItem item( layer, row, position, size, image );
    _items.append( item );
}

void RenderScene::addRect( RenderSceneLayerEnum layer, int row, const QPointF& position, const QSizeF& size, const QColor& color ) {
    RenderSceneItem item( layer, row, position, size, color );
    _items.append( item );
}

void RenderScene::build( QSGNode* rootNode, QQuickWindow* window, const Camera& camera, TextureCache& textureCache ) {
    if ( !rootNode || !window ) {
        return;
    }

    std::stable_sort( _items.begin(), _items.end(), []( const RenderSceneItem& left, const RenderSceneItem& right ) {
        const int leftBand = sortBand( left.layer() );
        const int rightBand = sortBand( right.layer() );

        if ( leftBand != rightBand ) {
            return leftBand < rightBand;
        }

        if ( leftBand == sortBand( RenderSceneLayerEnum::Object ) && left.row() != right.row() ) {
            return left.row() < right.row();
        }

        return left.layer() < right.layer();
    } );

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
