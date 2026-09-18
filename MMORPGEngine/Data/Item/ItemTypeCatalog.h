#ifndef ITEMTYPECATALOG_H
#define ITEMTYPECATALOG_H

#include <unordered_map>

#include <MMORPGEngine/Data/Item/ItemTypeModel.h>

namespace Engine {

class ItemTypeCatalog {
public:
    ItemTypeCatalog();

    const ItemTypeModel* itemType( uint32_t type ) const;
    const std::unordered_map<uint32_t, ItemTypeModel>& itemTypes() const;
    void addItemType( const ItemTypeModel& itemType );

private:
    std::unordered_map<uint32_t, ItemTypeModel> _itemTypes;
};

} // namespace Engine

#endif // ITEMTYPECATALOG_H
