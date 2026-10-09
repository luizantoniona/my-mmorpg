#ifndef CHARACTEREQUIPMENTSYSTEM_H
#define CHARACTEREQUIPMENTSYSTEM_H

#include <cstdint>
#include <optional>
#include <vector>

#include <MMORPGEngine/Data/Item/ItemTypeModel.h>
#include <MMORPGEngine/Entity/Character/CharacterEquipmentModel.h>
#include <MMORPGEngine/Entity/Character/CharacterInventoryModel.h>
#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/Character/EquipmentSlotEnum.h>
#include <MMORPGServer/Server/Event/WorldEvent.h>
#include <MMORPGServer/Server/Runtime/Character/System/CharacterSystem.h>

namespace Server {

class CharacterEquipmentSystem : public CharacterSystem {
public:
    CharacterEquipmentSystem( Engine::CharacterModel* character, std::vector<WorldEvent>& pendingEvents );

    void onTick() override;

    bool equip( uint32_t inventoryPosition );
    bool unequip( Engine::EquipmentSlotEnum slot );

private:
    static const Engine::ItemTypeModel* itemTypeOf( uint32_t idItem );
    static std::optional<Engine::EquipmentSlotEnum> targetSlot( const Engine::ItemTypeModel& itemType, const std::vector<Engine::CharacterEquipmentModel>& equipment );
    static std::vector<Engine::EquipmentSlotEnum> slotsToFree( const Engine::ItemTypeModel& itemType, Engine::EquipmentSlotEnum target, const std::vector<Engine::CharacterEquipmentModel>& equipment );
    static std::optional<uint32_t> firstFreePosition( const std::vector<Engine::CharacterInventoryModel>& inventory );
    static Engine::CharacterInventoryModel inventoryEntry( uint32_t position, uint32_t idItem );

    void commit( const std::vector<Engine::CharacterInventoryModel>& inventory, const std::vector<Engine::CharacterEquipmentModel>& equipment );

private:
    Engine::CharacterModel* _character;
    std::vector<WorldEvent>& _pendingEvents;
};

} // namespace Server

#endif // CHARACTEREQUIPMENTSYSTEM_H
