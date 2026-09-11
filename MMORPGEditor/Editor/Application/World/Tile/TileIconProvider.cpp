#include "TileIconProvider.h"

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Tile/TileCatalog.h>
#include <MMORPGEngine/Data/Tile/TileModel.h>

TileIconProvider::TileIconProvider() :
    QQuickImageProvider( QQuickImageProvider::Image ) {
}

QImage TileIconProvider::requestImage( const QString& id, QSize* size, const QSize& ) {
    const uint32_t type = id.toUInt();

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
