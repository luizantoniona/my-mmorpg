#include "EntityTextureModel.h"

#include <QPainter>

#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

const QImage& EntityTextureModel::characterTexture() {
    static const QImage texture = createPlaceholder( QColor( 66, 135, 245 ) );
    return texture;
}

const QImage& EntityTextureModel::creatureTexture() {
    static const QImage texture = createPlaceholder( QColor( 214, 61, 61 ) );
    return texture;
}

QImage EntityTextureModel::createPlaceholder( const QColor& color ) {
    const int size = WorldConstants::TILE_SIZE;

    QImage image( size, size, QImage::Format_ARGB32_Premultiplied );
    image.fill( Qt::transparent );

    QPainter painter( &image );
    painter.setRenderHint( QPainter::Antialiasing );
    painter.setPen( Qt::NoPen );
    painter.setBrush( color );
    painter.drawEllipse( image.rect().adjusted( 4, 4, -4, -4 ) );

    return image;
}

} // namespace Engine
