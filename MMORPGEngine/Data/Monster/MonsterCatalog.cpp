#include "MonsterCatalog.h"

namespace Engine {

MonsterCatalog::MonsterCatalog() :
    _monsters() {
}

const MonsterModel* MonsterCatalog::monster( uint32_t type ) const {
    auto iterator = _monsters.find( type );

    if ( iterator == _monsters.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

const std::unordered_map<uint32_t, MonsterModel>& MonsterCatalog::monsters() const {
    return _monsters;
}

void MonsterCatalog::addMonster( const MonsterModel& monster ) {
    _monsters.insert( { monster.type(), monster } );
}

} // namespace Engine
