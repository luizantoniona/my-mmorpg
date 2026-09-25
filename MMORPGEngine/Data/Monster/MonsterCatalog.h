#ifndef MONSTERCATALOG_H
#define MONSTERCATALOG_H

#include <unordered_map>

#include <MMORPGEngine/Data/Monster/MonsterModel.h>

namespace Engine {

class MonsterCatalog {
public:
    MonsterCatalog();

    const MonsterModel* monster( uint32_t type ) const;
    const std::unordered_map<uint32_t, MonsterModel>& monsters() const;
    void addMonster( const MonsterModel& monster );

private:
    std::unordered_map<uint32_t, MonsterModel> _monsters;
};

} // namespace Engine

#endif // MONSTERCATALOG_H
