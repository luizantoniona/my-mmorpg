#include "WorldGroundItemSystem.h"

#include <algorithm>
#include <cstdlib>
#include <mutex>

#include <QDebug>

#include <MMORPGServer/Server/Event/WorldEvent.h>
#include <MMORPGServer/Server/Event/WorldEventType.h>
#include <MMORPGServer/Server/Runtime/Character/CharacterRuntime.h>
#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace {

constexpr int PICKUP_REACH_TILES = 1;

} // namespace

namespace Server {

WorldGroundItemSystem::WorldGroundItemSystem( WorldRuntime& runtime ) :
    WorldSystem( runtime ),
    _nextIdGroundItem( 1 ) {
}

void WorldGroundItemSystem::onTick() {
}

void WorldGroundItemSystem::handle( const CharacterDropItemCommand& command ) {
    drop( command.idCharacter(), command.inventoryPosition() );
}

void WorldGroundItemSystem::handle( const CharacterPickupItemCommand& command ) {
    pickup( command.idCharacter(), command.idGroundItem() );
}

bool WorldGroundItemSystem::drop( int idCharacter, uint32_t inventoryPosition ) {
    std::vector<WorldEvent> events;

    {
        std::lock_guard<std::mutex> lock( _runtime.mutex() );

        CharacterRuntime* characterRuntime = _runtime.characterRuntimeLocked( idCharacter );
        if ( !characterRuntime ) {
            return false;
        }

        const std::optional<QString> idItem = characterRuntime->inventorySystem().take( inventoryPosition );
        if ( !idItem ) {
            return false;
        }

        const Engine::GroundItemModel groundItem = createGroundItem( *idItem, characterRuntime->character()->position() );
        events = characterRuntime->takePendingEvents();

        Json::Value payload;
        payload[ "idGroundItem" ] = groundItem.idGroundItem();
        payload[ "idItem" ] = groundItem.idItem().toStdString();
        payload[ "x" ] = groundItem.position().x();
        payload[ "y" ] = groundItem.position().y();
        payload[ "z" ] = groundItem.position().z();
        events.push_back( WorldEvent( WorldEventType::GROUND_ITEM_ENTERED, payload ) );

        qInfo() << "[WorldGroundItemSystem] Item dropped [CHARACTER]" << idCharacter << "[GROUND ITEM]" << groundItem.idGroundItem();
    }

    _runtime.eventBus().publish( events );

    return true;
}

bool WorldGroundItemSystem::pickup( int idCharacter, int idGroundItem ) {
    std::vector<WorldEvent> events;

    {
        std::lock_guard<std::mutex> lock( _runtime.mutex() );

        CharacterRuntime* characterRuntime = _runtime.characterRuntimeLocked( idCharacter );
        if ( !characterRuntime ) {
            return false;
        }

        const std::optional<Engine::GroundItemModel> groundItem = findLocked( idGroundItem );
        if ( !groundItem ) {
            qWarning() << "[WorldGroundItemSystem] Pickup rejected: unknown ground item [CHARACTER]" << idCharacter << "[GROUND ITEM]" << idGroundItem;
            return false;
        }

        if ( !isReachable( *groundItem, characterRuntime->character()->position() ) ) {
            qWarning() << "[WorldGroundItemSystem] Pickup rejected: out of reach [CHARACTER]" << idCharacter << "[GROUND ITEM]" << idGroundItem;
            return false;
        }

        if ( !characterRuntime->inventorySystem().add( groundItem->idItem() ) ) {
            return false;
        }

        eraseGroundItem( idGroundItem );
        events = characterRuntime->takePendingEvents();

        Json::Value payload;
        payload[ "idGroundItem" ] = groundItem->idGroundItem();
        payload[ "x" ] = groundItem->position().x();
        payload[ "y" ] = groundItem->position().y();
        payload[ "z" ] = groundItem->position().z();
        events.push_back( WorldEvent( WorldEventType::GROUND_ITEM_LEFT, payload ) );

        qInfo() << "[WorldGroundItemSystem] Item picked up [CHARACTER]" << idCharacter << "[GROUND ITEM]" << idGroundItem;
    }

    _runtime.eventBus().publish( events );

    return true;
}

std::optional<Engine::GroundItemModel> WorldGroundItemSystem::find( int idGroundItem ) const {
    std::lock_guard<std::mutex> lock( _runtime.mutex() );

    return findLocked( idGroundItem );
}

std::vector<Engine::GroundItemModel> WorldGroundItemSystem::itemsVisibleFrom( const Engine::EntityPositionModel& position ) const {
    std::lock_guard<std::mutex> lock( _runtime.mutex() );

    return itemsVisibleFromLocked( position );
}

std::optional<Engine::GroundItemModel> WorldGroundItemSystem::findLocked( int idGroundItem ) const {
    const auto it = _groundItems.find( idGroundItem );
    if ( it == _groundItems.end() ) {
        return std::nullopt;
    }

    return it->second;
}

std::vector<Engine::GroundItemModel> WorldGroundItemSystem::itemsVisibleFromLocked( const Engine::EntityPositionModel& position ) const {
    std::vector<Engine::GroundItemModel> result;

    for ( int idGroundItem : _runtime.spatialIndex().groundItemsVisibleFrom( position ) ) {
        const auto it = _groundItems.find( idGroundItem );

        if ( it != _groundItems.end() ) {
            result.push_back( it->second );
        }
    }

    return result;
}

bool WorldGroundItemSystem::isReachable( const Engine::GroundItemModel& groundItem, const Engine::EntityPositionModel& position ) {
    const Engine::EntityPositionModel& itemPosition = groundItem.position();

    if ( itemPosition.z() != position.z() ) {
        return false;
    }

    return std::max( std::abs( itemPosition.x() - position.x() ), std::abs( itemPosition.y() - position.y() ) ) <= PICKUP_REACH_TILES;
}

Engine::GroundItemModel WorldGroundItemSystem::createGroundItem( const QString& idItem, const Engine::EntityPositionModel& position ) {
    Engine::GroundItemModel groundItem;
    groundItem.setIdGroundItem( _nextIdGroundItem++ );
    groundItem.setIdItem( idItem );
    groundItem.position().setX( position.x() );
    groundItem.position().setY( position.y() );
    groundItem.position().setZ( position.z() );

    _groundItems[ groundItem.idGroundItem() ] = groundItem;
    _runtime.spatialIndex().addGroundItem( groundItem.idGroundItem(), groundItem.position() );

    return groundItem;
}

void WorldGroundItemSystem::eraseGroundItem( int idGroundItem ) {
    const auto it = _groundItems.find( idGroundItem );
    if ( it == _groundItems.end() ) {
        return;
    }

    _runtime.spatialIndex().removeGroundItem( idGroundItem, it->second.position() );
    _groundItems.erase( it );
}

} // namespace Server
