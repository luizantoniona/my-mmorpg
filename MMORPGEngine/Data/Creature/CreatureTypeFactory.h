#ifndef CREATURETYPEFACTORY_H
#define CREATURETYPEFACTORY_H

#include <QString>
#include <QStringList>

#include <MMORPGEngine/Data/Creature/CreatureTypeCatalog.h>

namespace Engine {

class CreatureTypeFactory {
public:
    static bool isValidType( const QString& type );
    static QStringList readTypeKeys( const QString& configPath );
    static QString typeFilePath( const QString& configPath, const QString& type );

    static void createCreatureTypeCatalog( const QString& configPath, CreatureTypeCatalog& creatureTypeCatalog );
    static void saveCreatureTypeCatalog( const QString& configPath, const CreatureTypeCatalog& creatureTypeCatalog );
};

} // namespace Engine

#endif // CREATURETYPEFACTORY_H
