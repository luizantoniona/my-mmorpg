#ifndef SKILLCATALOG_H
#define SKILLCATALOG_H

#include <map>

#include <QString>

#include <MMORPGEngine/Data/Skill/SkillTreeModel.h>

namespace Engine {

class SkillCatalog {
public:
    SkillCatalog();

    const SkillTreeModel* tree( const QString& itemType ) const;
    const std::map<QString, SkillTreeModel>& trees() const;
    void addTree( const SkillTreeModel& tree );

private:
    std::map<QString, SkillTreeModel> _trees;
};

} // namespace Engine

#endif // SKILLCATALOG_H
