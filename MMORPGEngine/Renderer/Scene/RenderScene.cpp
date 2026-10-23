#include "RenderScene.h"

#include <algorithm>

#include <QSGSimpleRectNode>
#include <QSGSimpleTextureNode>

namespace Engine {

RenderScene::RenderScene() {
}

RenderScene::~RenderScene() {
    clear();
}

void RenderScene::clear() {
    _tiles.clear();
    _objects.clear();
    _overlays.clear();
    _effects.clear();
    _releasedTextures.clear();
}

void RenderScene::addTexture( RenderSceneLayerEnum layer, int row, const QPointF& position, const QSizeF& size, const QImage& image ) {
    add( RenderSceneItem( layer, row, position, size, image ) );
}

void RenderScene::addRect( RenderSceneLayerEnum layer, int row, const QPointF& position, const QSizeF& size, const QColor& color ) {
    add( RenderSceneItem( layer, row, position, size, color ) );
}

void RenderScene::releaseTexture( qint64 cacheKey ) {
    _releasedTextures.append( cacheKey );
}

int RenderScene::itemCount() const {
    return static_cast<int>( _tiles.size() + _objects.size() + _overlays.size() + _effects.size() );
}

void RenderScene::add( const RenderSceneItem& item ) {
    switch ( item.layer() ) {
    case RenderSceneLayerEnum::Tile:
        _tiles.append( item );
        break;
    case RenderSceneLayerEnum::Object:
    case RenderSceneLayerEnum::Entity:
        _objects.append( item );
        break;
    case RenderSceneLayerEnum::Overlay:
        _overlays.append( item );
        break;
    case RenderSceneLayerEnum::Effect:
        _effects.append( item );
        break;
    }
}

void RenderScene::build( QSGNode* rootNode, QQuickWindow* window, const Camera& camera, TextureCache& textureCache ) {
    if ( !rootNode || !window ) {
        return;
    }

    std::stable_sort( _objects.begin(), _objects.end(), []( const RenderSceneItem& left, const RenderSceneItem& right ) {
        if ( left.row() != right.row() ) {
            return left.row() < right.row();
        }

        return left.layer() < right.layer();
    } );

    buildItems( _tiles, rootNode, window, camera, textureCache );
    buildItems( _objects, rootNode, window, camera, textureCache );
    buildItems( _overlays, rootNode, window, camera, textureCache );
    buildItems( _effects, rootNode, window, camera, textureCache );

    for ( qint64 cacheKey : _releasedTextures ) {
        textureCache.release( cacheKey );
    }
}

void RenderScene::buildItems( const QList<RenderSceneItem>& items, QSGNode* rootNode, QQuickWindow* window, const Camera& camera, TextureCache& textureCache ) const {
    for ( const RenderSceneItem& item : items ) {
        const QPointF screenPosition = camera.worldToScreen( item.position() );
        const QSizeF screenSize = item.size() * camera.zoom();

        if ( item.color().isValid() ) {
            auto* rectNode = new QSGSimpleRectNode();
            rectNode->setColor( item.color() );
            rectNode->setRect( screenPosition.x(), screenPosition.y(), screenSize.width(), screenSize.height() );

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
        node->setRect( screenPosition.x(), screenPosition.y(), screenSize.width(), screenSize.height() );

        rootNode->appendChildNode( node );
    }
}

} // namespace Engine
