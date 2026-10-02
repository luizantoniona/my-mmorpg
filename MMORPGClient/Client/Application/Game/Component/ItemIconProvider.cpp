#include "ItemIconProvider.h"

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Item/ItemCatalog.h>
#include <MMORPGEngine/Data/Item/ItemModel.h>

ItemIconProvider::ItemIconProvider() :
    QQuickImageProvider( QQuickImageProvider::Image ) {
    qInfo() << "ItemIconProvider::ItemIconProvider";
}

QImage ItemIconProvider::requestImage( const QString& id, QSize* size, const QSize& ) {
    const Engine::ItemCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().itemCatalog();
    const Engine::ItemModel* item = catalog.item( id.toUInt() );

    if ( !item ) {
        return QImage();
    }

    const QImage texture = item->texture();

    if ( size ) {
        *size = texture.size();
    }

    return texture;
}
