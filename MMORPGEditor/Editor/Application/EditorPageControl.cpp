#include "EditorPageControl.h"

#include <MMORPGEngine/Core/World/WorldFactory.h>

EditorPageControl::EditorPageControl( QObject* parent ) :
    QObject( parent ) {}

QString EditorPageControl::worldName() const {
    if ( !_world ) {
        return {};
    }

    return QString::fromStdString( _world->name() );
}

QVariantList EditorPageControl::grounds() const {
    QVariantList result;

    if ( !_world ) {
        return result;
    }

    const auto& catalog = _world->groundCatalog();

    for ( const auto& [id, ground] : catalog.grounds() ) {

        QVariantMap item;
        item["id"] = static_cast<int>( id );
        item["name"] = QString::fromStdString( ground.name() );
        item["walkable"] = ground.walkable();

        result.append( item );
    }

    return result;
}

void EditorPageControl::loadWorld( const QString& path ) {
    _world = Engine::WorldFactory::createWorld( path.toStdString() );

    emit worldChanged();
}
