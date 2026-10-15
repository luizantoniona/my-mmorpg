#ifndef ITEMCATALOG_H
#define ITEMCATALOG_H

#include <map>

#include <QString>

#include <MMORPGEngine/Data/Item/ItemModel.h>

namespace Engine {

class ItemCatalog {
public:
    ItemCatalog();

    const ItemModel* item( const QString& id ) const;
    const std::map<QString, ItemModel>& items() const;
    void addItem( const ItemModel& item );

private:
    std::map<QString, ItemModel> _items;
};

} // namespace Engine

#endif // ITEMCATALOG_H
