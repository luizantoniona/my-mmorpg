#ifndef CREATURETYPEFACTORY_H
#define CREATURETYPEFACTORY_H

#include <QString>

#include <MMORPGEngine/Data/Creature/CreatureTypeCatalog.h>

namespace Engine {

class CreatureTypeFactory {
public:
    static void createCreatureTypeCatalog( const QString& configPath, CreatureTypeCatalog& creatureTypeCatalog );
    static void saveCreatureTypeCatalog( const QString& configPath, const CreatureTypeCatalog& creatureTypeCatalog );
};

} // namespace Engine

#endif // CREATURETYPEFACTORY_H
