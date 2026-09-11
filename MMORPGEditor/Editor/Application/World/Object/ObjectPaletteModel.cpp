#include "ObjectPaletteModel.h"

#include <algorithm>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Object/ObjectCatalog.h>
#include <MMORPGEngine/Data/Object/ObjectModel.h>

ObjectPaletteModel::ObjectPaletteModel( QObject* parent ) :
    QAbstractListModel( parent ),
    _types() {

    _types.append( 0 );

    const Engine::ObjectCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().objectCatalog();

    for ( const auto& entry : catalog.objects() ) {
        _types.append( entry.first );
    }

    std::sort( _types.begin(), _types.end() );
}

int ObjectPaletteModel::rowCount( const QModelIndex& parent ) const {
    if ( parent.isValid() ) {
        return 0;
    }

    return _types.size();
}

QVariant ObjectPaletteModel::data( const QModelIndex& index, int role ) const {
    if ( !index.isValid() || index.row() < 0 || index.row() >= _types.size() ) {
        return QVariant();
    }

    const uint32_t type = _types.at( index.row() );

    if ( type == 0 ) {
        switch ( role ) {
        case TypeRole:
            return type;
        case NameRole:
            return QStringLiteral( "None" );
        case IconSourceRole:
            return QStringLiteral( "image://EditorObjectIcon/0" );
        default:
            return QVariant();
        }
    }

    const Engine::ObjectCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().objectCatalog();
    const Engine::ObjectModel* object = catalog.object( type );

    if ( !object ) {
        return QVariant();
    }

    switch ( role ) {
    case TypeRole:
        return type;
    case NameRole:
        return object->name();
    case IconSourceRole:
        return QString( "image://EditorObjectIcon/%1" ).arg( type );
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ObjectPaletteModel::roleNames() const {
    return { { TypeRole, "type" },
             { NameRole, "name" },
             { IconSourceRole, "iconSource" } };
}
