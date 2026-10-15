#include "ItemCatalog.h"

namespace Engine {

ItemCatalog::ItemCatalog() :
    _items() {
}

const ItemModel* ItemCatalog::item( const QString& id ) const {
    auto iterator = _items.find( id );

    if ( iterator == _items.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

const std::map<QString, ItemModel>& ItemCatalog::items() const {
    return _items;
}

void ItemCatalog::addItem( const ItemModel& item ) {
    _items.insert( { item.id(), item } );
}

} // namespace Engine
