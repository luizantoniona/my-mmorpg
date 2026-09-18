#ifndef ITEMTYPEFACTORY_H
#define ITEMTYPEFACTORY_H

#include <QString>

#include <MMORPGEngine/Data/Item/ItemTypeCatalog.h>

namespace Engine {

class ItemTypeFactory {
public:
    static void createItemTypeCatalog( const QString& configPath, ItemTypeCatalog& itemTypeCatalog );
    static void saveItemTypeCatalog( const QString& configPath, const ItemTypeCatalog& itemTypeCatalog );
};

} // namespace Engine

#endif // ITEMTYPEFACTORY_H
