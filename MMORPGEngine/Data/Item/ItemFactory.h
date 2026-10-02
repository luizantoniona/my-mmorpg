#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include <QString>

#include <MMORPGEngine/Data/Item/ItemCatalog.h>
#include <MMORPGEngine/Data/Item/ItemTypeCatalog.h>

namespace Engine {

class ItemFactory {
public:
    static void createItemCatalog( const QString& configPath, const ItemTypeCatalog& itemTypeCatalog, ItemCatalog& itemCatalog );
    static void saveItemCatalog( const QString& configPath, const ItemCatalog& itemCatalog );
};

} // namespace Engine

#endif // ITEMFACTORY_H
