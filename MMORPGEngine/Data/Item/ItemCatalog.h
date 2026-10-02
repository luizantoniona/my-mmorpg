#ifndef ITEMCATALOG_H
#define ITEMCATALOG_H

#include <unordered_map>

#include <MMORPGEngine/Data/Item/ItemModel.h>

namespace Engine {

class ItemCatalog {
public:
    ItemCatalog();

    const ItemModel* item( uint32_t id ) const;
    const std::unordered_map<uint32_t, ItemModel>& items() const;
    void addItem( const ItemModel& item );

private:
    std::unordered_map<uint32_t, ItemModel> _items;
};

} // namespace Engine

#endif // ITEMCATALOG_H
