#ifndef ITEMTYPEFACTORY_H
#define ITEMTYPEFACTORY_H

#include <QString>
#include <QStringList>

#include <MMORPGEngine/Data/Item/ItemTypeCatalog.h>

namespace Engine {

class ItemTypeFactory {
public:
    static bool isValidType( const QString& type );
    static QStringList readTypeKeys( const QString& configPath );
    static QString typeFilePath( const QString& configPath, const QString& type );

    static void createItemTypeCatalog( const QString& configPath, ItemTypeCatalog& itemTypeCatalog );
    static void saveItemTypeCatalog( const QString& configPath, const ItemTypeCatalog& itemTypeCatalog );
};

} // namespace Engine

#endif // ITEMTYPEFACTORY_H
