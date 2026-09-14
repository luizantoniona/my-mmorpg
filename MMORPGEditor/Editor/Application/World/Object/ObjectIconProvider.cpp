#include "ObjectIconProvider.h"

#include <QPainter>
#include <QPen>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Object/ObjectCatalog.h>
#include <MMORPGEngine/Data/Object/ObjectModel.h>
#include <MMORPGEngine/World/WorldConstants.h>

// TODO: Find other way
namespace {
QImage noneIcon() {
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

ObjectIconProvider::ObjectIconProvider() :
    QQuickImageProvider( QQuickImageProvider::Image ) {
    qInfo() << "ObjectIconProvider::ObjectIconProvider";
}

QImage ObjectIconProvider::requestImage( const QString& id, QSize* size, const QSize& ) {
    const uint32_t type = id.toUInt();

    if ( type == 0 ) {
        const QImage icon = noneIcon();

        if ( size ) {
            *size = icon.size();
        }

        return icon;
    }

    const Engine::ObjectCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().objectCatalog();
    const Engine::ObjectModel* object = catalog.object( type );

    if ( !object ) {
        return QImage();
    }

    const QImage texture = object->texture();

    if ( size ) {
        *size = texture.size();
    }

    return texture;
}
