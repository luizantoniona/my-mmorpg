#ifndef SKILLCATALOG_H
#define SKILLCATALOG_H

#include <unordered_map>

#include <MMORPGEngine/Data/Skill/SkillTreeModel.h>

namespace Engine {

class SkillCatalog {
public:
    SkillCatalog();

    const SkillTreeModel* tree( uint32_t itemType ) const;
    const std::unordered_map<uint32_t, SkillTreeModel>& trees() const;
    void addTree( const SkillTreeModel& tree );

private:
    std::unordered_map<uint32_t, SkillTreeModel> _trees;
};

} // namespace Engine

#endif // SKILLCATALOG_H
