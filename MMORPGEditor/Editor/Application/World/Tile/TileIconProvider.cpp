#include "TileIconProvider.h"

#include <QPainter>
#include <QPen>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Tile/TileCatalog.h>
#include <MMORPGEngine/Data/Tile/TileModel.h>
#include <MMORPGEngine/World/WorldConstants.h>

// TODO: Find other way
namespace {
QImage emptyIcon() {
    const int size = Engine::WorldConstants::TILE_SIZE;

    QImage icon( size, size, QImage::Format_ARGB32 );
    icon.fill( Qt::transparent );

    QPainter painter( &icon );
    painter.setRenderHint( QPainter::Antialiasing );
    painter.setPen( QPen( QColor( 217, 83, 79 ), 3 ) );
    painter.drawLine( 4, 4, size - 4, size - 4 );
    painter.drawLine( size - 4, 4, 4, size - 4 );

    return icon;
}
} // namespace

TileIconProvider::TileIconProvider() :
    QQuickImageProvider( QQuickImageProvider::Image ) {
    qInfo() << "TileIconProvider::TileIconProvider";
}

QImage TileIconProvider::requestImage( const QString& id, QSize* size, const QSize& ) {
    const uint32_t type = id.toUInt();

    if ( type == 0 ) {
        const QImage icon = emptyIcon();

        if ( size ) {
            *size = icon.size();
        }

        return icon;
    }

    const Engine::TileCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().tileCatalog();
    const Engine::TileModel* tile = catalog.tile( type );

    if ( !tile ) {
        return QImage();
    }

    const QImage texture = tile->texture();

    if ( size ) {
        *size = texture.size();
    }

    return texture;
}
