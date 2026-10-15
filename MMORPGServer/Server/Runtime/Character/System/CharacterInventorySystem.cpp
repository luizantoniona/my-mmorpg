#include "CharacterInventorySystem.h"

#include <algorithm>

#include <QDebug>

#include <MMORPGServer/Server/Event/WorldEventType.h>

namespace {

// TODO: capacidade provisória do inventário, espelha o vCapacity do Client; depende de Level como stat
constexpr uint32_t INVENTORY_CAPACITY = 20;

} // namespace

namespace Server {

CharacterInventorySystem::CharacterInventorySystem( Engine::CharacterModel* character, std::vector<WorldEvent>& pendingEvents ) :
    _character( character ),
    _pendingEvents( pendingEvents ) {
}

void CharacterInventorySystem::onTick() {
}

std::optional<QString> CharacterInventorySystem::take( uint32_t inventoryPosition ) {
    std::vector<Engine::CharacterInventoryModel> inventory = _character->inventory();

    auto source = std::find_if( inventory.begin(), inventory.end(), [ inventoryPosition ]( const Engine::CharacterInventoryModel& entry ) {
        return entry.position() == inventoryPosition;
    } );
    if ( source == inventory.end() ) {
        qWarning() << "[CharacterInventorySystem] Take rejected: empty position [CHARACTER]" << _character->idCharacter() << "[POSITION]" << inventoryPosition;
        return std::nullopt;
    }

    const QString idItem = source->idItem();

    if ( source->amount() > 1 ) {
        source->setAmount( source->amount() - 1 );
    } else {
        inventory.erase( source );
    }

    _character->setInventory( inventory );

    Json::Value payload;
    payload[ "idCharacter" ] = _character->idCharacter();
    _pendingEvents.push_back( WorldEvent( WorldEventType::CHARACTER_INVENTORY_CHANGED, payload ) );

    return idItem;
}

bool CharacterInventorySystem::add( const QString& idItem ) {
    std::vector<Engine::CharacterInventoryModel> inventory = _character->inventory();

    const std::optional<uint32_t> freePosition = firstFreePosition( inventory );
    if ( !freePosition ) {
        qWarning() << "[CharacterInventorySystem] Add rejected: inventory full [CHARACTER]" << _character->idCharacter();
        return false;
    }

    inventory.push_back( inventoryEntry( *freePosition, idItem ) );
    _character->setInventory( inventory );

    Json::Value payload;
    payload[ "idCharacter" ] = _character->idCharacter();
    _pendingEvents.push_back( WorldEvent( WorldEventType::CHARACTER_INVENTORY_CHANGED, payload ) );

    return true;
}

std::optional<uint32_t> CharacterInventorySystem::firstFreePosition( const std::vector<Engine::CharacterInventoryModel>& inventory ) {
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

Engine::CharacterInventoryModel CharacterInventorySystem::inventoryEntry( uint32_t position, const QString& idItem ) {
    Engine::CharacterInventoryModel entry;
    entry.setPosition( position );
    entry.setIdItem( idItem );
    entry.setAmount( 1 );

    return entry;
}

} // namespace Server
