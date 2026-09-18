#include "WorldRuntime.h"

#include <algorithm>

#include <QDebug>

#include <MMORPGEngine/World/WorldConstants.h>
#include <MMORPGServer/Server/Event/WorldEvent.h>
#include <MMORPGServer/Server/Event/WorldEventType.h>

namespace Server {

WorldRuntime::WorldRuntime( std::unique_ptr<Engine::WorldModel> world ) :
    _world( std::move( world ) ) {

    if ( !_world ) {
        return;
    }
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

Engine::CharacterModel* WorldRuntime::addCharacter( std::unique_ptr<Engine::CharacterModel> character ) {
    int idCharacter = 0;
    Engine::EntityPositionModel position;
    Engine::CharacterModel* characterPtr = nullptr;

    {
        std::lock_guard<std::mutex> lock( _mutex );

        idCharacter = character->idCharacter();
        position = character->position();

        auto characterRuntime = std::make_unique<CharacterRuntime>( std::move( character ), _eventBus );
        characterPtr = characterRuntime->character();
        _characters[ idCharacter ] = std::move( characterRuntime );

        _charactersByChunk[ chunkCoordinateFor( position ) ].push_back( characterPtr );

        qInfo() << "[WorldRuntime] Character added [CHARACTER]" << idCharacter << "[TOTAL]" << _characters.size();
    }

    Json::Value payload;
    payload[ "idCharacter" ] = idCharacter;
    payload[ "x" ] = position.x();
    payload[ "y" ] = position.y();
    payload[ "z" ] = position.z();

    _eventBus.publish( WorldEvent( WorldEventType::ENTITY_ENTERED, payload ) );

    return characterPtr;
}

void WorldRuntime::removeCharacter( int idCharacter ) {
    std::vector<int> nearbyCharacters;

    {
        std::lock_guard<std::mutex> lock( _mutex );

        auto it = _characters.find( idCharacter );
        if ( it != _characters.end() ) {
            Engine::CharacterModel* characterPtr = it->second->character();

            nearbyCharacters = charactersNearLocked( idCharacter );

            auto& bucket = _charactersByChunk[ chunkCoordinateFor( characterPtr->position() ) ];
            bucket.erase( std::remove( bucket.begin(), bucket.end(), characterPtr ), bucket.end() );
        }

        _characters.erase( idCharacter );

        qInfo() << "[WorldRuntime] Character removed [CHARACTER]" << idCharacter << "[TOTAL]" << _characters.size();
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

Engine::CharacterModel* WorldRuntime::character( int idCharacter ) {
    std::lock_guard<std::mutex> lock( _mutex );

    auto it = _characters.find( idCharacter );
    return it != _characters.end() ? it->second->character() : nullptr;
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

void WorldRuntime::moveCharacter( int idCharacter, int x, int y, int z ) {
    {
        std::lock_guard<std::mutex> lock( _mutex );

        auto it = _characters.find( idCharacter );
        if ( it == _characters.end() ) {
            return;
        }

        Engine::CharacterModel* characterPtr = it->second->character();

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

std::vector<int> WorldRuntime::charactersNear( int idCharacter ) {
    std::lock_guard<std::mutex> lock( _mutex );

    return charactersNearLocked( idCharacter );
}

void WorldRuntime::tick() {
    std::lock_guard<std::mutex> lock( _mutex );

    for ( auto& entry : _characters ) {
        entry.second->tick();
    }
}

ChunkCoordinate WorldRuntime::chunkCoordinateFor( const Engine::EntityPositionModel& position ) const {
    return ChunkCoordinate( position.x() / Engine::WorldConstants::CHUNK_SIZE, position.y() / Engine::WorldConstants::CHUNK_SIZE, position.z() );
}

std::vector<int> WorldRuntime::charactersNearLocked( int idCharacter ) const {
    std::vector<int> result;

    auto it = _characters.find( idCharacter );
    if ( it == _characters.end() ) {
        return result;
    }

    const ChunkCoordinate centerChunk = chunkCoordinateFor( it->second->character()->position() );

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
