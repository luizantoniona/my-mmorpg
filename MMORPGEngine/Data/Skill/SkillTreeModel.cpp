#include "SkillTreeModel.h"

namespace Engine {

SkillTreeModel::SkillTreeModel() :
    _nodes(),
    _itemType( 0 ) {
}

uint32_t SkillTreeModel::itemType() const {
    return _itemType;
}

void SkillTreeModel::setItemType( uint32_t itemType ) {
    _itemType = itemType;
}

const SkillNodeModel* SkillTreeModel::node( uint32_t type ) const {
    auto iterator = _nodes.find( type );

    if ( iterator == _nodes.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

const std::unordered_map<uint32_t, SkillNodeModel>& SkillTreeModel::nodes() const {
    return _nodes;
}

void SkillTreeModel::addNode( const SkillNodeModel& node ) {
    _nodes.insert( { node.type(), node } );
}

} // namespace Engine
