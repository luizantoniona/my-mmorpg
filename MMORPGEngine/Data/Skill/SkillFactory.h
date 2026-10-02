#ifndef SKILLFACTORY_H
#define SKILLFACTORY_H

#include <QString>

#include <MMORPGEngine/Data/Item/ItemTypeCatalog.h>
#include <MMORPGEngine/Data/Skill/SkillCatalog.h>

namespace Engine {

class SkillFactory {
public:
    static void createSkillCatalog( const QString& configPath, const ItemTypeCatalog& itemTypeCatalog, SkillCatalog& skillCatalog );
    static void saveSkillCatalog( const QString& configPath, const SkillCatalog& skillCatalog );
};

} // namespace Engine

#endif // SKILLFACTORY_H
