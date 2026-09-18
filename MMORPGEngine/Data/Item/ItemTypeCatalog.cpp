#include "ItemTypeCatalog.h"

namespace Engine {

ItemTypeCatalog::ItemTypeCatalog() :
    _itemTypes() {
}

const ItemTypeModel* ItemTypeCatalog::itemType( uint32_t type ) const {
    auto iterator = _itemTypes.find( type );

    if ( iterator == _itemTypes.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

const std::unordered_map<uint32_t, ItemTypeModel>& ItemTypeCatalog::itemTypes() const {
    return _itemTypes;
}

void ItemTypeCatalog::addItemType( const ItemTypeModel& itemType ) {
    _itemTypes.insert( { itemType.type(), itemType } );
}

} // namespace Engine
