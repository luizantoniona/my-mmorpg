#include "TilePaletteModel.h"

#include <algorithm>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Tile/TileCatalog.h>
#include <MMORPGEngine/Data/Tile/TileModel.h>

TilePaletteModel::TilePaletteModel( QObject* parent ) :
    QAbstractListModel( parent ),
    _types() {

    const Engine::TileCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().tileCatalog();

    for ( const auto& entry : catalog.tiles() ) {
        _types.append( entry.first );
    }

    std::sort( _types.begin(), _types.end() );
}

int TilePaletteModel::rowCount( const QModelIndex& parent ) const {
    if ( parent.isValid() ) {
        return 0;
    }

    return _types.size();
}

QVariant TilePaletteModel::data( const QModelIndex& index, int role ) const {
    if ( !index.isValid() || index.row() < 0 || index.row() >= _types.size() ) {
        return QVariant();
    }

    const uint32_t type = _types.at( index.row() );

    const Engine::TileCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().tileCatalog();
    const Engine::TileModel* tile = catalog.tile( type );

    if ( !tile ) {
        return QVariant();
    }

    switch ( role ) {
    case TypeRole:
        return type;
    case NameRole:
        return tile->name();
    case IconSourceRole:
        return QString( "image://EditorTileIcon/%1" ).arg( type );
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> TilePaletteModel::roleNames() const {
    return { { TypeRole, "type" },
             { NameRole, "name" },
             { IconSourceRole, "iconSource" } };
}
