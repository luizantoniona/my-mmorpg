#include "SkillCatalog.h"

namespace Engine {

SkillCatalog::SkillCatalog() :
    _trees() {
}

const SkillTreeModel* SkillCatalog::tree( const QString& itemType ) const {
    auto iterator = _trees.find( itemType );

    if ( iterator == _trees.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

const std::map<QString, SkillTreeModel>& SkillCatalog::trees() const {
    return _trees;
}

void SkillCatalog::addTree( const SkillTreeModel& tree ) {
    _trees.insert( { tree.itemType(), tree } );
}

} // namespace Engine
