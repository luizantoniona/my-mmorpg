#ifndef WORLDGROUNDITEMSYSTEM_H
#define WORLDGROUNDITEMSYSTEM_H

#include <cstdint>
#include <map>
#include <optional>
#include <vector>

#include <QString>

#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/Entity/GroundItem/GroundItemModel.h>
#include <MMORPGServer/Server/Runtime/World/Command/Character/CharacterDropItemCommand.h>
#include <MMORPGServer/Server/Runtime/World/Command/Character/CharacterPickupItemCommand.h>
#include <MMORPGServer/Server/Runtime/World/System/WorldSystem.h>

namespace Server {

class WorldGroundItemSystem : public WorldSystem {
public:
    explicit WorldGroundItemSystem( WorldRuntime& runtime );

    void onTick() override;

    void handle( const CharacterDropItemCommand& command );
    void handle( const CharacterPickupItemCommand& command );

    bool drop( int idCharacter, uint32_t inventoryPosition );
    bool pickup( int idCharacter, int idGroundItem );

    std::optional<Engine::GroundItemModel> find( int idGroundItem ) const;
    std::vector<Engine::GroundItemModel> itemsVisibleFrom( const Engine::EntityPositionModel& position ) const;

    std::optional<Engine::GroundItemModel> findLocked( int idGroundItem ) const;
    std::vector<Engine::GroundItemModel> itemsVisibleFromLocked( const Engine::EntityPositionModel& position ) const;

    static bool isReachable( const Engine::GroundItemModel& groundItem, const Engine::EntityPositionModel& position );

private:
    Engine::GroundItemModel createGroundItem( const QString& idItem, const Engine::EntityPositionModel& position );
    void eraseGroundItem( int idGroundItem );

private:
    std::map<int, Engine::GroundItemModel> _groundItems;
    int _nextIdGroundItem;
};

} // namespace Server

#endif // WORLDGROUNDITEMSYSTEM_H
