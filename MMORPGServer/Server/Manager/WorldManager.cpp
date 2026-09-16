#include "WorldManager.h"

#include <algorithm>
#include <chrono>

#include <QDebug>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/World/WorldConstants.h>
#include <MMORPGEngine/World/WorldFactory.h>

namespace Server {

WorldManager::WorldManager() :
    _running( false ),
    _thread(),
    _world( nullptr ) {
}

WorldManager::~WorldManager() {
}

void WorldManager::initialize( const std::string& worldPath ) {
    if ( !_world ) {
        _world = Engine::WorldFactory::createWorld( worldPath );
    }

    if ( _running ) {
        return;
    }

    // TODO: Create WorldConfigurationManager and store tickRate;
    const int tickRate = 20;
    const int msPerTick = 1000 / tickRate;

    _running = true;
    _thread = std::thread( [ this, msPerTick ]() {
        using clock = std::chrono::steady_clock;
        auto nextTick = clock::now();

        while ( _running ) {
            nextTick += std::chrono::milliseconds( msPerTick );

            // TODO: Tick runtime world and thread systems

            std::this_thread::sleep_until( nextTick );
        }
    } );
}

void WorldManager::finalize() {
    _running = false;

    if ( _thread.joinable() ) {
        _thread.join();
    }
}

Engine::WorldModel* WorldManager::world() {
    return _world.get();
}

const Engine::WorldModel* WorldManager::world() const {
    return _world.get();
}

EventBus& WorldManager::eventBus() {
    return _eventBus;
}

Engine::CharacterModel* WorldManager::addCharacter( std::unique_ptr<Engine::CharacterModel> character ) {
    int idCharacter = 0;
    Engine::EntityPositionModel position;
    Engine::CharacterModel* characterPtr = nullptr;

    {
        std::lock_guard<std::mutex> lock( _mutex );

        idCharacter = character->idCharacter();
        characterPtr = character.get();
        position = characterPtr->position();
        _characters[ idCharacter ] = std::move( character );

        _charactersByChunk[ chunkCoordinateFor( position ) ].push_back( characterPtr );

        qInfo() << "[WorldManager] Character added [CHARACTER]" << idCharacter << "[TOTAL]" << _characters.size();
    }

    Json::Value payload;
    payload[ "idCharacter" ] = idCharacter;
    payload[ "x" ] = position.x();
    payload[ "y" ] = position.y();
    payload[ "z" ] = position.z();

    _eventBus.publish( WorldEvent( WorldEventType::ENTITY_ENTERED, payload ) );

    return characterPtr;
}

void WorldManager::removeCharacter( int idCharacter ) {
    std::vector<int> nearbyCharacters;

    {
        std::lock_guard<std::mutex> lock( _mutex );

        auto it = _characters.find( idCharacter );
        if ( it != _characters.end() ) {
            Engine::CharacterModel* characterPtr = it->second.get();

            nearbyCharacters = charactersNearLocked( idCharacter );

            auto& bucket = _charactersByChunk[ chunkCoordinateFor( characterPtr->position() ) ];
            bucket.erase( std::remove( bucket.begin(), bucket.end(), characterPtr ), bucket.end() );
        }

        _characters.erase( idCharacter );

        qInfo() << "[WorldManager] Character removed [CHARACTER]" << idCharacter << "[TOTAL]" << _characters.size();
    }

    if ( nearbyCharacters.empty() ) {
        return;
    }

    Json::Value nearbyJson( Json::arrayValue );
    for ( int nearbyIdCharacter : nearbyCharacters ) {
        nearbyJson.append( nearbyIdCharacter );
    }

    Json::Value payload;
    payload[ "idCharacter" ] = idCharacter;
    payload[ "nearby" ] = nearbyJson;

    _eventBus.publish( WorldEvent( WorldEventType::ENTITY_LEFT, payload ) );
}

Engine::CharacterModel* WorldManager::character( int idCharacter ) {
    std::lock_guard<std::mutex> lock( _mutex );

    auto it = _characters.find( idCharacter );
    return it != _characters.end() ? it->second.get() : nullptr;
}

std::map<int, Engine::EntityPositionModel> WorldManager::characterPositions() {
    std::lock_guard<std::mutex> lock( _mutex );

    std::map<int, Engine::EntityPositionModel> positions;
    for ( const auto& entry : _characters ) {
        positions[ entry.first ] = entry.second->position();
    }

    return positions;
}

std::vector<Engine::CharacterModel> WorldManager::connectedCharacters() {
    std::lock_guard<std::mutex> lock( _mutex );

    std::vector<Engine::CharacterModel> result;
    result.reserve( _characters.size() );

    for ( const auto& entry : _characters ) {
        result.push_back( *entry.second );
    }

    return result;
}

void WorldManager::moveCharacter( int idCharacter, int x, int y, int z ) {
    {
        std::lock_guard<std::mutex> lock( _mutex );

        auto it = _characters.find( idCharacter );
        if ( it == _characters.end() ) {
            return;
        }

        Engine::CharacterModel* characterPtr = it->second.get();

        const ChunkCoordinate previousChunk = chunkCoordinateFor( characterPtr->position() );

        Engine::EntityPositionModel position;
        position.setX( x );
        position.setY( y );
        position.setZ( z );
        characterPtr->setPosition( position );

        const ChunkCoordinate newChunk = chunkCoordinateFor( position );

        if ( !( previousChunk == newChunk ) ) {
            auto& previousBucket = _charactersByChunk[ previousChunk ];
            previousBucket.erase( std::remove( previousBucket.begin(), previousBucket.end(), characterPtr ), previousBucket.end() );

            _charactersByChunk[ newChunk ].push_back( characterPtr );
        }
    }

    Json::Value payload;
    payload[ "idCharacter" ] = idCharacter;
    payload[ "x" ] = x;
    payload[ "y" ] = y;
    payload[ "z" ] = z;

    _eventBus.publish( WorldEvent( WorldEventType::ENTITY_MOVED, payload ) );
}

std::vector<int> WorldManager::charactersNear( int idCharacter ) {
    std::lock_guard<std::mutex> lock( _mutex );

    return charactersNearLocked( idCharacter );
}

ChunkCoordinate WorldManager::chunkCoordinateFor( const Engine::EntityPositionModel& position ) const {
    return ChunkCoordinate( position.x() / Engine::WorldConstants::CHUNK_SIZE, position.y() / Engine::WorldConstants::CHUNK_SIZE, position.z() );
}

std::vector<int> WorldManager::charactersNearLocked( int idCharacter ) const {
    std::vector<int> result;

    auto it = _characters.find( idCharacter );
    if ( it == _characters.end() ) {
        return result;
    }

    const ChunkCoordinate centerChunk = chunkCoordinateFor( it->second->position() );

    for ( int dx = -1; dx <= 1; ++dx ) {
        for ( int dy = -1; dy <= 1; ++dy ) {
            const ChunkCoordinate neighborChunk( centerChunk.x + dx, centerChunk.y + dy, centerChunk.z );

            auto chunkIt = _charactersByChunk.find( neighborChunk );
            if ( chunkIt == _charactersByChunk.end() ) {
                continue;
            }

            for ( Engine::CharacterModel* characterPtr : chunkIt->second ) {
                if ( characterPtr->idCharacter() == idCharacter ) {
                    continue;
                }

                result.push_back( characterPtr->idCharacter() );
            }
        }
    }

    return result;
}

} // namespace Server
