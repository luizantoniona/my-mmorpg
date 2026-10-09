#include "EntityTextureModel.h"

#include <algorithm>
#include <map>

#include <QPainter>

#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

const QImage& EntityTextureModel::characterTexture() {
    static const QImage texture = createPlaceholder( QColor( 66, 135, 245 ) );
    return texture;
}

const QImage& EntityTextureModel::creatureTexture( int footprint ) {
    static std::map<int, QImage> textures;

    const int clampedFootprint = std::max( footprint, 1 );

    auto it = textures.find( clampedFootprint );
    if ( it == textures.end() ) {
        it = textures.emplace( clampedFootprint, createPlaceholder( QColor( 214, 61, 61 ), clampedFootprint ) ).first;
    }

    return it->second;
}

QImage EntityTextureModel::createPlaceholder( const QColor& color, int footprint ) {
    const int size = WorldConstants::TILE_SIZE * footprint;

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
