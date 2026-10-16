#include "WorldRuntime.h"

#include <algorithm>
#include <iterator>

#include <QDebug>

#include <MMORPGEngine/World/WorldConstants.h>
#include <MMORPGServer/Server/Event/WorldEvent.h>
#include <MMORPGServer/Server/Event/WorldEventType.h>

namespace Server {

WorldRuntime::WorldRuntime( std::unique_ptr<Engine::WorldModel> world, int tickRate ) :
    _world( std::move( world ) ),
    _spatialIndex( _characters, _creatures ),
    _combatSystem( std::make_unique<WorldCombatSystem>( *this ) ),
    _movementSystem( std::make_unique<WorldMovementSystem>( *this ) ),
    _creatureSystem( std::make_unique<WorldCreatureSystem>( *this ) ),
    _spawnSystem( std::make_unique<WorldSpawnSystem>( *this ) ),
    _tickRate( tickRate ) {
}

Engine::WorldModel* WorldRuntime::world() {
    return _world.get();
}

const Engine::WorldModel* WorldRuntime::world() const {
    return _world.get();
}

EventBus& WorldRuntime::eventBus() {
    return _eventBus;
}

std::mutex& WorldRuntime::mutex() {
    return _mutex;
}

WorldSpatialIndex& WorldRuntime::spatialIndex() {
    return _spatialIndex;
}

const WorldSpatialIndex& WorldRuntime::spatialIndex() const {
    return _spatialIndex;
}

WorldCombatSystem& WorldRuntime::combatSystem() {
    return *_combatSystem;
}

WorldMovementSystem& WorldRuntime::movementSystem() {
    return *_movementSystem;
}

WorldSpawnSystem& WorldRuntime::spawnSystem() {
    return *_spawnSystem;
}

int WorldRuntime::tickRate() const {
    return _tickRate;
}

Engine::CharacterModel* WorldRuntime::addCharacter( std::unique_ptr<Engine::CharacterModel> character ) {
    int idCharacter = 0;
    Engine::EntityPositionModel position;
    Engine::CharacterModel* characterPtr = nullptr;

    {
        std::lock_guard<std::mutex> lock( _mutex );

        idCharacter = character->idCharacter();

        if ( _characters.find( idCharacter ) != _characters.end() ) {
            qWarning() << "[WorldRuntime] Character already in world, add rejected [CHARACTER]" << idCharacter;
            return nullptr;
        }

        position = character->position();

        auto characterRuntime = std::make_unique<CharacterRuntime>( std::move( character ), _eventBus, _tickRate );
        characterPtr = characterRuntime->character();
        _characters[ idCharacter ] = std::move( characterRuntime );

        _spatialIndex.addCharacter( characterPtr );

        qInfo() << "[WorldRuntime] Character added [CHARACTER]" << idCharacter << "[TOTAL]" << _characters.size();
    }

    Json::Value payload;
    payload[ "idCharacter" ] = idCharacter;
    payload[ "x" ] = position.x();
    payload[ "y" ] = position.y();
    payload[ "z" ] = position.z();

    _eventBus.publish( WorldEvent( WorldEventType::CHARACTER_ENTERED, payload ) );

    return characterPtr;
}

void WorldRuntime::removeCharacter( int idCharacter ) {
    std::vector<int> nearbyCharacters;
    bool removed = false;

    {
        std::lock_guard<std::mutex> lock( _mutex );

        auto it = _characters.find( idCharacter );
        if ( it != _characters.end() ) {
            nearbyCharacters = _spatialIndex.charactersNear( idCharacter );

            _spatialIndex.removeCharacter( it->second->character() );

            removed = true;
        }

        _characters.erase( idCharacter );

        qInfo() << "[WorldRuntime] Character removed [CHARACTER]" << idCharacter << "[TOTAL]" << _characters.size();
    }

    if ( !removed ) {
        return;
    }

    Json::Value nearbyJson( Json::arrayValue );
    for ( int nearbyIdCharacter : nearbyCharacters ) {
        nearbyJson.append( nearbyIdCharacter );
    }

    Json::Value payload;
    payload[ "idCharacter" ] = idCharacter;
    payload[ "nearby" ] = nearbyJson;

    _eventBus.publish( WorldEvent( WorldEventType::CHARACTER_LEFT, payload ) );
}

Engine::CharacterModel* WorldRuntime::character( int idCharacter ) {
    std::lock_guard<std::mutex> lock( _mutex );

    return characterLocked( idCharacter );
}

bool WorldRuntime::equipCharacter( int idCharacter, uint32_t inventoryPosition ) {
    std::vector<WorldEvent> events;
    bool equipped = false;

    {
        std::lock_guard<std::mutex> lock( _mutex );

        auto it = _characters.find( idCharacter );
        if ( it == _characters.end() ) {
            return false;
        }

        equipped = it->second->equipmentSystem().equip( inventoryPosition );
        events = it->second->takePendingEvents();
    }

    for ( const WorldEvent& event : events ) {
        _eventBus.publish( event );
    }

    return equipped;
}

bool WorldRuntime::unequipCharacter( int idCharacter, Engine::EquipmentSlotEnum slot ) {
    std::vector<WorldEvent> events;
    bool unequipped = false;

    {
        std::lock_guard<std::mutex> lock( _mutex );

        auto it = _characters.find( idCharacter );
        if ( it == _characters.end() ) {
            return false;
        }

        unequipped = it->second->equipmentSystem().unequip( slot );
        events = it->second->takePendingEvents();
    }

    for ( const WorldEvent& event : events ) {
        _eventBus.publish( event );
    }

    return unequipped;
}

bool WorldRuntime::dropCharacterItem( int idCharacter, uint32_t inventoryPosition ) {
    std::vector<WorldEvent> events;

    {
        std::lock_guard<std::mutex> lock( _mutex );

        auto it = _characters.find( idCharacter );
        if ( it == _characters.end() ) {
            return false;
        }

        const std::optional<QString> idItem = it->second->inventorySystem().take( inventoryPosition );
        if ( !idItem ) {
            return false;
        }

        const Engine::GroundItemModel groundItem = _groundItemSystem.drop( *idItem, it->second->character()->position() );
        events = it->second->takePendingEvents();

        Json::Value payload;
        payload[ "idGroundItem" ] = groundItem.idGroundItem();
        payload[ "idItem" ] = groundItem.idItem().toStdString();
        payload[ "x" ] = groundItem.position().x();
        payload[ "y" ] = groundItem.position().y();
        payload[ "z" ] = groundItem.position().z();
        events.push_back( WorldEvent( WorldEventType::GROUND_ITEM_ENTERED, payload ) );

        qInfo() << "[WorldRuntime] Item dropped [CHARACTER]" << idCharacter << "[GROUND ITEM]" << groundItem.idGroundItem();
    }

    for ( const WorldEvent& event : events ) {
        _eventBus.publish( event );
    }

    return true;
}

bool WorldRuntime::pickupCharacterItem( int idCharacter, int idGroundItem ) {
    std::vector<WorldEvent> events;

    {
        std::lock_guard<std::mutex> lock( _mutex );

        auto it = _characters.find( idCharacter );
        if ( it == _characters.end() ) {
            return false;
        }

        const std::optional<Engine::GroundItemModel> groundItem = _groundItemSystem.find( idGroundItem );
        if ( !groundItem ) {
            qWarning() << "[WorldRuntime] Pickup rejected: unknown ground item [CHARACTER]" << idCharacter << "[GROUND ITEM]" << idGroundItem;
            return false;
        }

        if ( !WorldGroundItemSystem::isReachable( *groundItem, it->second->character()->position() ) ) {
            qWarning() << "[WorldRuntime] Pickup rejected: out of reach [CHARACTER]" << idCharacter << "[GROUND ITEM]" << idGroundItem;
            return false;
        }

        if ( !it->second->inventorySystem().add( groundItem->idItem() ) ) {
            return false;
        }

        _groundItemSystem.remove( idGroundItem );
        events = it->second->takePendingEvents();

        Json::Value payload;
        payload[ "idGroundItem" ] = groundItem->idGroundItem();
        payload[ "x" ] = groundItem->position().x();
        payload[ "y" ] = groundItem->position().y();
        payload[ "z" ] = groundItem->position().z();
        events.push_back( WorldEvent( WorldEventType::GROUND_ITEM_LEFT, payload ) );

        qInfo() << "[WorldRuntime] Item picked up [CHARACTER]" << idCharacter << "[GROUND ITEM]" << idGroundItem;
    }

    for ( const WorldEvent& event : events ) {
        _eventBus.publish( event );
    }

    return true;
}

std::vector<Engine::GroundItemModel> WorldRuntime::groundItemsNear( const Engine::EntityPositionModel& position ) {
    std::lock_guard<std::mutex> lock( _mutex );

    return _groundItemSystem.itemsNear( position );
}

std::optional<Engine::GroundItemModel> WorldRuntime::groundItem( int idGroundItem ) {
    std::lock_guard<std::mutex> lock( _mutex );

    return _groundItemSystem.find( idGroundItem );
}

std::vector<ChunkCoordinate> WorldRuntime::terrainChunksNear( const Engine::EntityPositionModel& position ) const {
    std::vector<ChunkCoordinate> result;

    if ( !_world ) {
        return result;
    }

    const int chunkCountX = ( static_cast<int>( _world->width() ) + Engine::WorldConstants::CHUNK_SIZE - 1 ) / Engine::WorldConstants::CHUNK_SIZE;
    const int chunkCountY = ( static_cast<int>( _world->height() ) + Engine::WorldConstants::CHUNK_SIZE - 1 ) / Engine::WorldConstants::CHUNK_SIZE;

    const int centerX = position.x() / Engine::WorldConstants::CHUNK_SIZE;
    const int centerY = position.y() / Engine::WorldConstants::CHUNK_SIZE;

    for ( int chunkX = centerX - 1; chunkX <= centerX + 1; ++chunkX ) {
        for ( int chunkY = centerY - 1; chunkY <= centerY + 1; ++chunkY ) {
            if ( chunkX >= 0 && chunkY >= 0 && chunkX < chunkCountX && chunkY < chunkCountY ) {
                result.push_back( ChunkCoordinate( chunkX, chunkY, 0 ) );
            }
        }
    }

    return result;
}

WorldVisibilityDelta WorldRuntime::visibilityDelta( const Engine::EntityPositionModel& previousPosition, const Engine::EntityPositionModel& currentPosition, int ignoredIdCharacter ) {
    std::lock_guard<std::mutex> lock( _mutex );

    const auto sorted = []( std::vector<int> ids ) {
        std::sort( ids.begin(), ids.end() );
        return ids;
    };

    const auto difference = [ &sorted ]( const std::vector<int>& from, const std::vector<int>& other ) {
        const std::vector<int> sortedFrom = sorted( from );
        const std::vector<int> sortedOther = sorted( other );

        std::vector<int> result;
        std::set_difference( sortedFrom.begin(), sortedFrom.end(), sortedOther.begin(), sortedOther.end(), std::back_inserter( result ) );

        return result;
    };

    const auto withoutIgnored = [ ignoredIdCharacter ]( std::vector<int> ids ) {
        ids.erase( std::remove( ids.begin(), ids.end(), ignoredIdCharacter ), ids.end() );
        return ids;
    };

    const auto creatureIds = [ this ]( const Engine::EntityPositionModel& position ) {
        std::vector<int> ids;
        for ( const Engine::CreatureModel& creature : _spatialIndex.creaturesNear( position ) ) {
            ids.push_back( creature.idCreature() );
        }
        return ids;
    };

    const auto groundItemIds = [ this ]( const Engine::EntityPositionModel& position ) {
        std::vector<int> ids;
        for ( const Engine::GroundItemModel& groundItem : _groundItemSystem.itemsNear( position ) ) {
            ids.push_back( groundItem.idGroundItem() );
        }
        return ids;
    };

    const std::vector<int> previousCharacters = withoutIgnored( _spatialIndex.charactersNear( previousPosition ) );
    const std::vector<int> currentCharacters = withoutIgnored( _spatialIndex.charactersNear( currentPosition ) );
    const std::vector<int> previousCreatures = creatureIds( previousPosition );
    const std::vector<int> currentCreatures = creatureIds( currentPosition );
    const std::vector<int> previousGroundItems = groundItemIds( previousPosition );
    const std::vector<int> currentGroundItems = groundItemIds( currentPosition );

    WorldVisibilityDelta delta;
    delta.charactersEntered = difference( currentCharacters, previousCharacters );
    delta.charactersLeft = difference( previousCharacters, currentCharacters );
    delta.creaturesEntered = difference( currentCreatures, previousCreatures );
    delta.creaturesLeft = difference( previousCreatures, currentCreatures );
    delta.groundItemsEntered = difference( currentGroundItems, previousGroundItems );
    delta.groundItemsLeft = difference( previousGroundItems, currentGroundItems );

    const std::vector<ChunkCoordinate> previousChunks = terrainChunksNear( previousPosition );
    const std::vector<ChunkCoordinate> currentChunks = terrainChunksNear( currentPosition );

    for ( const ChunkCoordinate& chunk : currentChunks ) {
        if ( std::find( previousChunks.begin(), previousChunks.end(), chunk ) == previousChunks.end() ) {
            delta.terrainChunksEntered.push_back( chunk );
        }
    }

    for ( const ChunkCoordinate& chunk : previousChunks ) {
        if ( std::find( currentChunks.begin(), currentChunks.end(), chunk ) == currentChunks.end() ) {
            delta.terrainChunksLeft.push_back( chunk );
        }
    }

    return delta;
}

std::map<int, Engine::EntityPositionModel> WorldRuntime::characterPositions() {
    std::lock_guard<std::mutex> lock( _mutex );

    std::map<int, Engine::EntityPositionModel> positions;
    for ( const auto& entry : _characters ) {
        positions[ entry.first ] = entry.second->character()->position();
    }

    return positions;
}

std::vector<Engine::CharacterModel> WorldRuntime::connectedCharacters() {
    std::lock_guard<std::mutex> lock( _mutex );

    std::vector<Engine::CharacterModel> result;
    result.reserve( _characters.size() );

    for ( const auto& entry : _characters ) {
        result.push_back( *entry.second->character() );
    }

    return result;
}

Engine::CreatureModel* WorldRuntime::addCreature( std::unique_ptr<Engine::CreatureModel> creature ) {
    return addCreature( std::move( creature ), nullptr );
}

Engine::CreatureModel* WorldRuntime::addCreature( std::unique_ptr<Engine::CreatureModel> creature, const WorldSpawnAreaRuntime* spawnArea ) {
    std::lock_guard<std::mutex> lock( _mutex );

    const int idCreature = creature->idCreature();

    auto creatureRuntime = std::make_unique<CreatureRuntime>( std::move( creature ), spawnArea );
    Engine::CreatureModel* creaturePtr = creatureRuntime->creature();
    _creatures[ idCreature ] = std::move( creatureRuntime );

    qInfo() << "[WorldRuntime] Creature added [CREATURE]" << idCreature << "[TOTAL]" << _creatures.size();

    return creaturePtr;
}

Engine::CreatureModel* WorldRuntime::creature( int idCreature ) {
    std::lock_guard<std::mutex> lock( _mutex );

    auto it = _creatures.find( idCreature );
    return it != _creatures.end() ? it->second->creature() : nullptr;
}

Engine::CreatureModel* WorldRuntime::creatureAt( int x, int y, int z ) {
    std::lock_guard<std::mutex> lock( _mutex );

    return _spatialIndex.creatureAt( x, y, z );
}

std::vector<Engine::CreatureModel> WorldRuntime::creatures() {
    std::lock_guard<std::mutex> lock( _mutex );

    std::vector<Engine::CreatureModel> result;
    result.reserve( _creatures.size() );

    for ( const auto& entry : _creatures ) {
        result.push_back( *entry.second->creature() );
    }

    return result;
}

std::vector<Engine::CreatureModel> WorldRuntime::creaturesNear( const Engine::EntityPositionModel& position ) {
    std::lock_guard<std::mutex> lock( _mutex );

    return _spatialIndex.creaturesNear( position );
}

Engine::CharacterModel* WorldRuntime::characterLocked( int idCharacter ) const {
    auto it = _characters.find( idCharacter );
    return it != _characters.end() ? it->second->character() : nullptr;
}

std::map<int, std::unique_ptr<CreatureRuntime>>& WorldRuntime::creaturesLocked() {
    return _creatures;
}

void WorldRuntime::eraseCreatureLocked( int idCreature ) {
    _creatures.erase( idCreature );
}

std::vector<int> WorldRuntime::charactersNear( int idCharacter ) {
    std::lock_guard<std::mutex> lock( _mutex );

    return _spatialIndex.charactersNear( idCharacter );
}

std::vector<int> WorldRuntime::charactersNear( const Engine::EntityPositionModel& position ) {
    std::lock_guard<std::mutex> lock( _mutex );

    return _spatialIndex.charactersNear( position );
}

bool WorldRuntime::isPositionOccupied( int x, int y, int z ) {
    std::lock_guard<std::mutex> lock( _mutex );

    Engine::EntityPositionModel position;
    position.setX( x );
    position.setY( y );
    position.setZ( z );

    return _spatialIndex.isPositionOccupied( position );
}

bool WorldRuntime::isAreaOccupied( int x, int y, int z, int width, int height ) {
    std::lock_guard<std::mutex> lock( _mutex );

    return _spatialIndex.isAreaOccupied( x, y, z, width, height );
}

void WorldRuntime::enqueueCommand( std::unique_ptr<WorldCommand> command ) {
    std::lock_guard<std::mutex> lock( _commandMutex );

    _commands.push_back( std::move( command ) );
}

void WorldRuntime::tick() {
    std::vector<std::unique_ptr<WorldCommand>> commands;

    {
        std::lock_guard<std::mutex> lock( _commandMutex );

        commands.swap( _commands );
    }

    for ( const std::unique_ptr<WorldCommand>& command : commands ) {
        command->execute( *this );
    }

    std::vector<WorldEvent> characterEvents;

    {
        std::lock_guard<std::mutex> lock( _mutex );

        for ( auto& entry : _characters ) {
            entry.second->tick();

            std::vector<WorldEvent> events = entry.second->takePendingEvents();
            characterEvents.insert( characterEvents.end(), events.begin(), events.end() );
        }
    }

    for ( const WorldEvent& event : characterEvents ) {
        _eventBus.publish( event );
    }

    _combatSystem->onTick();
    _movementSystem->onTick();
    _spawnSystem->onTick();
    _creatureSystem->onTick();
}

} // namespace Server
