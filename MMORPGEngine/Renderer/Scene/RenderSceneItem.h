#ifndef RENDERSCENEITEM_H
#define RENDERSCENEITEM_H

#include <QColor>
#include <QImage>
#include <QPointF>
#include <QSizeF>

#include <MMORPGEngine/Renderer/Scene/RenderSceneLayerEnum.h>

namespace Engine {

class RenderSceneItem {
public:
    RenderSceneItem();
    RenderSceneItem( RenderSceneLayerEnum layer, const QPointF& position, const QSizeF& size, const QImage& image );
    RenderSceneItem( RenderSceneLayerEnum layer, const QPointF& position, const QSizeF& size, const QColor& color );

    RenderSceneLayerEnum layer() const;
    void setLayer( RenderSceneLayerEnum layer );

    QPointF position() const;
    void setPosition( QPointF position );

    QSizeF size() const;
    void setSize( const QSizeF& size );

    QImage image() const;
    void setImage( const QImage& image );

    QColor color() const;
    void setColor( const QColor& color );

private:
    QPointF _position;
    QSizeF _size;
    QImage _image;
    QColor _color;
    RenderSceneLayerEnum _layer;
};

} // namespace Engine

#endif // RENDERSCENEITEM_H
