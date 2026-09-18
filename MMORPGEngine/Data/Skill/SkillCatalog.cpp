#include "SkillCatalog.h"

namespace Engine {

SkillCatalog::SkillCatalog() :
    _trees() {
}

const SkillTreeModel* SkillCatalog::tree( uint32_t itemType ) const {
    auto iterator = _trees.find( itemType );

    if ( iterator == _trees.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

const std::unordered_map<uint32_t, SkillTreeModel>& SkillCatalog::trees() const {
    return _trees;
}

void SkillCatalog::addTree( const SkillTreeModel& tree ) {
    _trees.insert( { tree.itemType(), tree } );
}

} // namespace Engine
