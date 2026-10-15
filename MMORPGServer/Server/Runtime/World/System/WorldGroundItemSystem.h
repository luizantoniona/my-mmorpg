#ifndef WORLDGROUNDITEMSYSTEM_H
#define WORLDGROUNDITEMSYSTEM_H

#include <cstdint>
#include <map>
#include <optional>
#include <vector>

#include <QString>

#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/Entity/GroundItem/GroundItemModel.h>

namespace Server {

class WorldGroundItemSystem {
public:
    WorldGroundItemSystem();

    Engine::GroundItemModel drop( const QString& idItem, const Engine::EntityPositionModel& position );
    bool remove( int idGroundItem );

    std::optional<Engine::GroundItemModel> find( int idGroundItem ) const;
    std::vector<Engine::GroundItemModel> itemsNear( const Engine::EntityPositionModel& position ) const;

    static bool isReachable( const Engine::GroundItemModel& groundItem, const Engine::EntityPositionModel& position );

private:
    std::map<int, Engine::GroundItemModel> _groundItems;
    int _nextIdGroundItem;
};

} // namespace Server

#endif // WORLDGROUNDITEMSYSTEM_H
