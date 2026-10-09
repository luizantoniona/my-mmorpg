#include "CharacterEquipmentSystem.h"

#include <algorithm>

#include <QDebug>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGServer/Server/Event/WorldEventType.h>
#include <MMORPGServer/Server/Runtime/Character/CharacterCombatActionResolver.h>

namespace {

// TODO: capacidade provisória do inventário, espelha o vCapacity do Client; depende de Level como stat
constexpr uint32_t INVENTORY_CAPACITY = 20;

bool hasSlot( const std::vector<Engine::CharacterEquipmentModel>& equipment, Engine::EquipmentSlotEnum slot ) {
    return std::any_of( equipment.begin(), equipment.end(), [ slot ]( const Engine::CharacterEquipmentModel& entry ) {
        return entry.slot() == slot;
    } );
}

bool isTwoHanded( const Engine::ItemTypeModel& itemType ) {
    return itemType.handRequirement().has_value() && itemType.handRequirement().value() == Engine::HandRequirementEnum::TWO_HANDED;
}

} // namespace

namespace Server {

CharacterEquipmentSystem::CharacterEquipmentSystem( Engine::CharacterModel* character, std::vector<WorldEvent>& pendingEvents ) :
    _character( character ),
    _pendingEvents( pendingEvents ) {
}

void CharacterEquipmentSystem::onTick() {
}

bool CharacterEquipmentSystem::equip( uint32_t inventoryPosition ) {
    const int idCharacter = _character->idCharacter();

    std::vector<Engine::CharacterInventoryModel> inventory = _character->inventory();
    std::vector<Engine::CharacterEquipmentModel> equipment = _character->equipment();

    auto source = std::find_if( inventory.begin(), inventory.end(), [ inventoryPosition ]( const Engine::CharacterInventoryModel& entry ) {
        return entry.position() == inventoryPosition;
    } );
    if ( source == inventory.end() ) {
        qWarning() << "[CharacterEquipmentSystem] Equip rejected: empty position [CHARACTER]" << idCharacter << "[POSITION]" << inventoryPosition;
        return false;
    }

    const uint32_t idItem = source->idItem();
    const Engine::ItemTypeModel* itemType = itemTypeOf( idItem );
    if ( !itemType ) {
        qWarning() << "[CharacterEquipmentSystem] Equip rejected: unknown item [CHARACTER]" << idCharacter << "[ITEM]" << idItem;
        return false;
    }

    const std::optional<Engine::EquipmentSlotEnum> target = targetSlot( *itemType, equipment );
    if ( !target ) {
        qWarning() << "[CharacterEquipmentSystem] Equip rejected: item has no equipment slot [CHARACTER]" << idCharacter << "[ITEM]" << idItem;
        return false;
    }

    std::vector<uint32_t> displacedItems;
    for ( const Engine::EquipmentSlotEnum slot : slotsToFree( *itemType, *target, equipment ) ) {
        auto occupied = std::find_if( equipment.begin(), equipment.end(), [ slot ]( const Engine::CharacterEquipmentModel& entry ) {
            return entry.slot() == slot;
        } );

        if ( occupied != equipment.end() ) {
            displacedItems.push_back( occupied->idItem() );
            equipment.erase( occupied );
        }
    }

    if ( source->amount() > 1 ) {
        source->setAmount( source->amount() - 1 );
    } else {
        inventory.erase( source );
    }

    for ( const uint32_t displacedItem : displacedItems ) {
        const std::optional<uint32_t> freePosition = firstFreePosition( inventory );
        if ( !freePosition ) {
            qWarning() << "[CharacterEquipmentSystem] Equip rejected: inventory full [CHARACTER]" << idCharacter;
            return false;
        }

        inventory.push_back( inventoryEntry( *freePosition, displacedItem ) );
    }

    Engine::CharacterEquipmentModel equipped;
    equipped.setSlot( *target );
    equipped.setIdItem( idItem );
    equipment.push_back( equipped );

    commit( inventory, equipment );

    qInfo() << "[CharacterEquipmentSystem] Item equipped [CHARACTER]" << idCharacter << "[ITEM]" << idItem;

    return true;
}

bool CharacterEquipmentSystem::unequip( Engine::EquipmentSlotEnum slot ) {
    const int idCharacter = _character->idCharacter();

    std::vector<Engine::CharacterInventoryModel> inventory = _character->inventory();
    std::vector<Engine::CharacterEquipmentModel> equipment = _character->equipment();

    auto occupied = std::find_if( equipment.begin(), equipment.end(), [ slot ]( const Engine::CharacterEquipmentModel& entry ) {
        return entry.slot() == slot;
    } );
    if ( occupied == equipment.end() ) {
        qWarning() << "[CharacterEquipmentSystem] Unequip rejected: empty slot [CHARACTER]" << idCharacter;
        return false;
    }

    const std::optional<uint32_t> freePosition = firstFreePosition( inventory );
    if ( !freePosition ) {
        qWarning() << "[CharacterEquipmentSystem] Unequip rejected: inventory full [CHARACTER]" << idCharacter;
        return false;
    }

    const uint32_t idItem = occupied->idItem();
    equipment.erase( occupied );
    inventory.push_back( inventoryEntry( *freePosition, idItem ) );

    commit( inventory, equipment );

    qInfo() << "[CharacterEquipmentSystem] Item unequipped [CHARACTER]" << idCharacter << "[ITEM]" << idItem;

    return true;
}

const Engine::ItemTypeModel* CharacterEquipmentSystem::itemTypeOf( uint32_t idItem ) {
    const Engine::DataManager& dataManager = Engine::Singleton<Engine::DataManager>::instance();

    const Engine::ItemModel* item = dataManager.itemCatalog().item( idItem );
    if ( !item ) {
        return nullptr;
    }

    return dataManager.itemTypeCatalog().itemType( item->idItemType() );
}

std::optional<Engine::EquipmentSlotEnum> CharacterEquipmentSystem::targetSlot( const Engine::ItemTypeModel& itemType, const std::vector<Engine::CharacterEquipmentModel>& equipment ) {
    switch ( itemType.slot() ) {
    case Engine::ItemSlotEnum::HAND:
        return itemType.category() == Engine::ItemCategoryEnum::WEAPON ? Engine::EquipmentSlotEnum::RIGHT_HAND : Engine::EquipmentSlotEnum::LEFT_HAND;
    case Engine::ItemSlotEnum::HEAD:
        return Engine::EquipmentSlotEnum::HEAD;
    case Engine::ItemSlotEnum::CHEST:
        return Engine::EquipmentSlotEnum::CHEST;
    case Engine::ItemSlotEnum::LEGS:
        return Engine::EquipmentSlotEnum::LEGS;
    case Engine::ItemSlotEnum::FEET:
        return Engine::EquipmentSlotEnum::FEET;
    case Engine::ItemSlotEnum::GLOVE:
        return Engine::EquipmentSlotEnum::GLOVE;
    case Engine::ItemSlotEnum::NECK:
        return Engine::EquipmentSlotEnum::NECK;
    case Engine::ItemSlotEnum::RING:
        if ( !hasSlot( equipment, Engine::EquipmentSlotEnum::LEFT_RING ) ) {
            return Engine::EquipmentSlotEnum::LEFT_RING;
        }
        return hasSlot( equipment, Engine::EquipmentSlotEnum::RIGHT_RING ) ? Engine::EquipmentSlotEnum::LEFT_RING : Engine::EquipmentSlotEnum::RIGHT_RING;
    case Engine::ItemSlotEnum::UNKNOWN:
        return std::nullopt;
    }

    return std::nullopt;
}

std::vector<Engine::EquipmentSlotEnum> CharacterEquipmentSystem::slotsToFree( const Engine::ItemTypeModel& itemType, Engine::EquipmentSlotEnum target, const std::vector<Engine::CharacterEquipmentModel>& equipment ) {
    std::vector<Engine::EquipmentSlotEnum> freedSlots = { target };

    if ( isTwoHanded( itemType ) && target == Engine::EquipmentSlotEnum::RIGHT_HAND ) {
        freedSlots.push_back( Engine::EquipmentSlotEnum::LEFT_HAND );
    }

    if ( target == Engine::EquipmentSlotEnum::LEFT_HAND ) {
        for ( const Engine::CharacterEquipmentModel& entry : equipment ) {
            const Engine::ItemTypeModel* equippedType = itemTypeOf( entry.idItem() );

            if ( entry.slot() == Engine::EquipmentSlotEnum::RIGHT_HAND && equippedType && isTwoHanded( *equippedType ) ) {
                freedSlots.push_back( Engine::EquipmentSlotEnum::RIGHT_HAND );
            }
        }
    }

    return freedSlots;
}

std::optional<uint32_t> CharacterEquipmentSystem::firstFreePosition( const std::vector<Engine::CharacterInventoryModel>& inventory ) {
    for ( uint32_t position = 0; position < INVENTORY_CAPACITY; ++position ) {
        const bool occupied = std::any_of( inventory.begin(), inventory.end(), [ position ]( const Engine::CharacterInventoryModel& entry ) {
            return entry.position() == position;
        } );

        if ( !occupied ) {
            return position;
        }
    }

    return std::nullopt;
}

Engine::CharacterInventoryModel CharacterEquipmentSystem::inventoryEntry( uint32_t position, uint32_t idItem ) {
    Engine::CharacterInventoryModel entry;
    entry.setPosition( position );
    entry.setIdItem( idItem );
    entry.setAmount( 1 );

    return entry;
}

void CharacterEquipmentSystem::commit( const std::vector<Engine::CharacterInventoryModel>& inventory, const std::vector<Engine::CharacterEquipmentModel>& equipment ) {
    _character->setInventory( inventory );
    _character->setEquipment( equipment );
    _character->combat().setAvailableActions( CharacterCombatActionResolver::resolve( equipment ) );

    Json::Value payload;
    payload[ "idCharacter" ] = _character->idCharacter();
    _pendingEvents.push_back( WorldEvent( WorldEventType::CHARACTER_EQUIPMENT_CHANGED, payload ) );
}

} // namespace Server
