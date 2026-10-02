#ifndef SKILLTREEMODEL_H
#define SKILLTREEMODEL_H

#include <cstdint>
#include <unordered_map>

#include <QString>

#include <MMORPGEngine/Data/Skill/SkillNodeModel.h>

namespace Engine {

class SkillTreeModel {
public:
    SkillTreeModel();

    QString itemType() const;
    void setItemType( const QString& itemType );

    const SkillNodeModel* node( uint32_t type ) const;
    const std::unordered_map<uint32_t, SkillNodeModel>& nodes() const;
    void addNode( const SkillNodeModel& node );

private:
    std::unordered_map<uint32_t, SkillNodeModel> _nodes;
    QString _itemType;
};

} // namespace Engine

#endif // SKILLTREEMODEL_H
