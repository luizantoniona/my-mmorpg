#include "WorldRuntime.h"

#include <QDebug>

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
