#ifndef MONSTERFACTORY_H
#define MONSTERFACTORY_H

#include <QString>

#include <MMORPGEngine/Data/Monster/MonsterCatalog.h>

namespace Engine {

class MonsterFactory {
public:
    static void createMonsterCatalog( const QString& configPath, MonsterCatalog& monsterCatalog );
    static void saveMonsterCatalog( const QString& configPath, const MonsterCatalog& monsterCatalog );
};

} // namespace Engine

#endif // MONSTERFACTORY_H
