#include "RenderSceneItem.h"

namespace Engine {

RenderSceneItem::RenderSceneItem() :
    _position( 0, 0 ),
    _size( 0, 0 ),
    _image(),
    _color() {
}

RenderSceneItem::RenderSceneItem( const QPointF& position, const QSizeF& size, const QImage& image ) :
    _position( position ),
    _size( size ),
    _image( image ),
    _color() {
}

RenderSceneItem::RenderSceneItem( const QPointF& position, const QSizeF& size, const QColor& color ) :
    _position( position ),
    _size( size ),
    _image(),
    _color( color ) {
}

QPointF RenderSceneItem::position() const {
    return _position;
}

void RenderSceneItem::setPosition( QPointF position ) {
    _position = position;
}

QSizeF RenderSceneItem::size() const {
    return _size;
}

void RenderSceneItem::setSize( const QSizeF& size ) {
    _size = size;
}

QImage RenderSceneItem::image() const {
    return _image;
}

void RenderSceneItem::setImage( const QImage& image ) {
    _image = image;
}

QColor RenderSceneItem::color() const {
    return _color;
}

void RenderSceneItem::setColor( const QColor& color ) {
    _color = color;
}

} // namespace Engine
