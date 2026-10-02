#ifndef ITEMTYPECATALOG_H
#define ITEMTYPECATALOG_H

#include <map>

#include <QString>

#include <MMORPGEngine/Data/Item/ItemTypeModel.h>

namespace Engine {

class ItemTypeCatalog {
public:
    ItemTypeCatalog();

    const ItemTypeModel* itemType( const QString& type ) const;
    const std::map<QString, ItemTypeModel>& itemTypes() const;
    void addItemType( const ItemTypeModel& itemType );

private:
    std::map<QString, ItemTypeModel> _itemTypes;
};

} // namespace Engine

#endif // ITEMTYPECATALOG_H
