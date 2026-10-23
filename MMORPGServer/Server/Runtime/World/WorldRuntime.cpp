#include "WorldRuntime.h"

#include <QDebug>

#include <MMORPGServer/Server/Event/WorldEvent.h>
#include <MMORPGServer/Server/Event/WorldEventType.h>
#include <MMORPGServer/Server/Runtime/World/Command/Character/CharacterAttackEntityCommand.h>
#include <MMORPGServer/Server/Runtime/World/Command/Character/CharacterDropItemCommand.h>
#include <MMORPGServer/Server/Runtime/World/Command/Character/CharacterEquipItemCommand.h>
#include <MMORPGServer/Server/Runtime/World/Command/Character/CharacterMoveCommand.h>
#include <MMORPGServer/Server/Runtime/World/Command/Character/CharacterPickupItemCommand.h>
#include <MMORPGServer/Server/Runtime/World/Command/Character/CharacterUnequipItemCommand.h>

namespace Server {

WorldRuntime::WorldRuntime( std::unique_ptr<Engine::WorldModel> world, int tickRate ) :
    _world( std::move( world ) ),
    _spatialIndex( _characters, _creatures ),
    _combatSystem( std::make_unique<WorldCombatSystem>( *this ) ),
    _groundItemSystem( std::make_unique<WorldGroundItemSystem>( *this ) ),
    _movementSystem( std::make_unique<WorldMovementSystem>( *this ) ),
    _creatureSystem( std::make_unique<WorldCreatureSystem>( *this ) ),
    _spawnSystem( std::make_unique<WorldSpawnSystem>( *this ) ),
    _visibilitySystem( std::make_unique<WorldVisibilitySystem>( *this ) ),
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

WorldGroundItemSystem& WorldRuntime::groundItemSystem() {
    return *_groundItemSystem;
}

WorldMovementSystem& WorldRuntime::movementSystem() {
    return *_movementSystem;
}

WorldSpawnSystem& WorldRuntime::spawnSystem() {
    return *_spawnSystem;
}

WorldVisibilitySystem& WorldRuntime::visibilitySystem() {
    return *_visibilitySystem;
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

    {
        std::lock_guard<std::mutex> lock( _mutex );

        auto it = _characters.find( idCharacter );
        if ( it == _characters.end() ) {
            return;
        }

        nearbyCharacters = _spatialIndex.charactersViewing( idCharacter );

        _spatialIndex.removeCharacter( it->second->character() );
        _characters.erase( it );

        qInfo() << "[WorldRuntime] Character removed [CHARACTER]" << idCharacter << "[TOTAL]" << _characters.size();
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
    _spatialIndex.addCreature( creatureRuntime.get() );
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

std::vector<Engine::CreatureModel> WorldRuntime::creaturesVisibleFrom( const Engine::EntityPositionModel& position ) {
    std::lock_guard<std::mutex> lock( _mutex );

    return _spatialIndex.creaturesVisibleFrom( position );
}

Engine::CharacterModel* WorldRuntime::characterLocked( int idCharacter ) const {
    auto it = _characters.find( idCharacter );
    return it != _characters.end() ? it->second->character() : nullptr;
}

CharacterRuntime* WorldRuntime::characterRuntimeLocked( int idCharacter ) {
    auto it = _characters.find( idCharacter );
    return it != _characters.end() ? it->second.get() : nullptr;
}

std::map<int, std::unique_ptr<CreatureRuntime>>& WorldRuntime::creaturesLocked() {
    return _creatures;
}

void WorldRuntime::eraseCreatureLocked( int idCreature ) {
    const auto it = _creatures.find( idCreature );
    if ( it == _creatures.end() ) {
        return;
    }

    _spatialIndex.removeCreature( it->second.get() );
    _creatures.erase( it );
}

std::vector<int> WorldRuntime::charactersVisibleFrom( int idCharacter ) {
    std::lock_guard<std::mutex> lock( _mutex );

    return _spatialIndex.charactersVisibleFrom( idCharacter );
}

std::vector<int> WorldRuntime::charactersViewing( int idCharacter ) {
    std::lock_guard<std::mutex> lock( _mutex );

    return _spatialIndex.charactersViewing( idCharacter );
}

std::vector<int> WorldRuntime::charactersViewing( const Engine::EntityPositionModel& position ) {
    std::lock_guard<std::mutex> lock( _mutex );

    return _spatialIndex.charactersViewing( position );
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
        dispatch( *command );
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

    _eventBus.publish( characterEvents );

    _combatSystem->onTick();
    _movementSystem->onTick();
    _spawnSystem->onTick();
    _creatureSystem->onTick();
}

void WorldRuntime::dispatchToCharacter( int idCharacter, const std::function<void( CharacterRuntime& )>& action ) {
    std::vector<WorldEvent> events;

    {
        std::lock_guard<std::mutex> lock( _mutex );

        CharacterRuntime* characterRuntime = characterRuntimeLocked( idCharacter );
        if ( !characterRuntime ) {
            return;
        }

        action( *characterRuntime );
        events = characterRuntime->takePendingEvents();
    }

    _eventBus.publish( events );
}

void WorldRuntime::dispatch( const WorldCommand& command ) {
    switch ( command.type() ) {
    case WorldCommandTypeEnum::CHARACTER_MOVE:
        _movementSystem->handle( static_cast<const CharacterMoveCommand&>( command ) );
        break;
    case WorldCommandTypeEnum::CHARACTER_ATTACK_ENTITY:
        _combatSystem->handle( static_cast<const CharacterAttackEntityCommand&>( command ) );
        break;
    case WorldCommandTypeEnum::CHARACTER_EQUIP_ITEM: {
        const auto& equip = static_cast<const CharacterEquipItemCommand&>( command );
        dispatchToCharacter( equip.idCharacter(), [ &equip ]( CharacterRuntime& characterRuntime ) {
            characterRuntime.equipmentSystem().equip( equip.inventoryPosition() );
        } );
        break;
    }
    case WorldCommandTypeEnum::CHARACTER_UNEQUIP_ITEM: {
        const auto& unequip = static_cast<const CharacterUnequipItemCommand&>( command );
        dispatchToCharacter( unequip.idCharacter(), [ &unequip ]( CharacterRuntime& characterRuntime ) {
            characterRuntime.equipmentSystem().unequip( unequip.slot() );
        } );
        break;
    }
    case WorldCommandTypeEnum::CHARACTER_DROP_ITEM:
        _groundItemSystem->handle( static_cast<const CharacterDropItemCommand&>( command ) );
        break;
    case WorldCommandTypeEnum::CHARACTER_PICKUP_ITEM:
        _groundItemSystem->handle( static_cast<const CharacterPickupItemCommand&>( command ) );
        break;
    }
}

} // namespace Server
