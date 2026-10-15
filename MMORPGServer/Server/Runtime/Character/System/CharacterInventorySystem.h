#ifndef CHARACTERINVENTORYSYSTEM_H
#define CHARACTERINVENTORYSYSTEM_H

#include <cstdint>
#include <optional>
#include <vector>

#include <QString>

#include <MMORPGEngine/Entity/Character/CharacterInventoryModel.h>
#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGServer/Server/Event/WorldEvent.h>
#include <MMORPGServer/Server/Runtime/Character/System/CharacterSystem.h>

namespace Server {

class CharacterInventorySystem : public CharacterSystem {
public:
    CharacterInventorySystem( Engine::CharacterModel* character, std::vector<WorldEvent>& pendingEvents );

    void onTick() override;

    std::optional<QString> take( uint32_t inventoryPosition );
    bool add( const QString& idItem );

    static std::optional<uint32_t> firstFreePosition( const std::vector<Engine::CharacterInventoryModel>& inventory );
    static Engine::CharacterInventoryModel inventoryEntry( uint32_t position, const QString& idItem );

private:
    Engine::CharacterModel* _character;
    std::vector<WorldEvent>& _pendingEvents;
};

} // namespace Server

#endif // CHARACTERINVENTORYSYSTEM_H
