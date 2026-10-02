#include "CreatureTypePaletteModel.h"

#include <algorithm>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Creature/CreatureTypeCatalog.h>
#include <MMORPGEngine/Data/Creature/CreatureTypeModel.h>
#include <MMORPGEngine/Data/DataManager.h>

CreatureTypePaletteModel::CreatureTypePaletteModel( QObject* parent ) :
    QAbstractListModel( parent ),
    _types() {

    const Engine::CreatureTypeCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().creatureTypeCatalog();

    for ( const auto& entry : catalog.creatureTypes() ) {
        _types.append( entry.first );
    }

    std::sort( _types.begin(), _types.end() );
}

int CreatureTypePaletteModel::rowCount( const QModelIndex& parent ) const {
    if ( parent.isValid() ) {
        return 0;
    }

    return _types.size();
}

QVariant CreatureTypePaletteModel::data( const QModelIndex& index, int role ) const {
    if ( !index.isValid() || index.row() < 0 || index.row() >= _types.size() ) {
        return QVariant();
    }

    const uint32_t type = _types.at( index.row() );

    const Engine::CreatureTypeCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().creatureTypeCatalog();
    const Engine::CreatureTypeModel* creatureType = catalog.creatureType( type );

    if ( !creatureType ) {
        return QVariant();
    }

    switch ( role ) {
    case TypeRole:
        return type;
    case NameRole:
        return creatureType->name();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> CreatureTypePaletteModel::roleNames() const {
    return { { TypeRole, "type" },
             { NameRole, "name" } };
}
