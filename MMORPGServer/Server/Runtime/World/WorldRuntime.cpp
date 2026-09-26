#include "WorldRuntime.h"

#include <algorithm>
#include <random>

#include <QDebug>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Creature/CreatureTypeModel.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Entity/EntityMovementModel.h>
#include <MMORPGEngine/World/WorldConstants.h>
#include <MMORPGServer/Server/Event/WorldEvent.h>
#include <MMORPGServer/Server/Event/WorldEventType.h>

namespace Server {

WorldRuntime::WorldRuntime( std::unique_ptr<Engine::WorldModel> world ) :
    _world( std::move( world ) ),
    _nextIdCreature( 1 ) {

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

        characterPtr->position().setX( x );
        characterPtr->position().setY( y );
        characterPtr->position().setZ( z );

        characterPtr->movement().setCounter( 0 );

        const ChunkCoordinate newChunk = chunkCoordinateFor( characterPtr->position() );

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

bool WorldRuntime::attackCreature( int idCharacter, int idCreature, double damage, double staminaCost ) {
    bool creatureDied = false;

    {
        std::lock_guard<std::mutex> lock( _mutex );

        auto characterIt = _characters.find( idCharacter );
        auto creatureIt = _creatures.find( idCreature );

        if ( characterIt == _characters.end() || creatureIt == _creatures.end() ) {
            return false;
        }

        Engine::CharacterModel* characterPtr = characterIt->second->character();
        Engine::CreatureModel* creaturePtr = creatureIt->second->creature();

        characterPtr->vitals().setStamina( std::max( 0.0, characterPtr->vitals().stamina() - staminaCost ) );
        characterPtr->combat().setCounter( 0 );

        creaturePtr->vitals().setHealth( std::max( 0.0, creaturePtr->vitals().health() - damage ) );

        creatureDied = creaturePtr->vitals().health() <= 0.0;

        if ( creatureDied ) {
            _creatures.erase( creatureIt );
        }
    }

    Json::Value characterPayload;
    characterPayload[ "idCharacter" ] = idCharacter;
    _eventBus.publish( WorldEvent( WorldEventType::ENTITY_VITALS_CHANGED, characterPayload ) );

    Json::Value creaturePayload;
    creaturePayload[ "idCreature" ] = idCreature;
    _eventBus.publish( WorldEvent( creatureDied ? WorldEventType::CREATURE_LEFT : WorldEventType::CREATURE_VITALS_CHANGED, creaturePayload ) );

    return true;
}

std::vector<int> WorldRuntime::charactersNear( int idCharacter ) {
    std::lock_guard<std::mutex> lock( _mutex );

    return charactersNearLocked( idCharacter );
}

bool WorldRuntime::isPositionOccupied( int x, int y, int z ) {
    std::lock_guard<std::mutex> lock( _mutex );

    Engine::EntityPositionModel position;
    position.setX( x );
    position.setY( y );
    position.setZ( z );

    return isPositionOccupiedLocked( position );
}

Engine::CreatureModel* WorldRuntime::addCreature( std::unique_ptr<Engine::CreatureModel> creature ) {
    std::lock_guard<std::mutex> lock( _mutex );

    const int idCreature = creature->idCreature();

    auto creatureRuntime = std::make_unique<CreatureRuntime>( std::move( creature ) );
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

    for ( auto& entry : _creatures ) {
        Engine::CreatureModel* creaturePtr = entry.second->creature();
        const Engine::EntityPositionModel& position = creaturePtr->position();

        if ( position.x() == x && position.y() == y && position.z() == z ) {
            return creaturePtr;
        }
    }

    return nullptr;
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

void WorldRuntime::spawnCreaturesFromAreas() {
    if ( !_world ) {
        return;
    }

    std::mt19937 randomEngine( std::random_device{}() );

    for ( int z : _world->floors() ) {
        for ( const Engine::CreatureSpawnAreaModel& area : _world->spawnAreas( z ) ) {
            if ( area.width() == 0 || area.height() == 0 ) {
                continue;
            }

            std::uniform_int_distribution<int> xDistribution( area.x(), area.x() + static_cast<int>( area.width() ) - 1 );
            std::uniform_int_distribution<int> yDistribution( area.y(), area.y() + static_cast<int>( area.height() ) - 1 );

            for ( const Engine::CreatureSpawnEntryModel& entry : area.creatures() ) {
                const Engine::CreatureTypeModel* creatureType = Engine::Singleton<Engine::DataManager>::instance().creatureTypeCatalog().creatureType( entry.type() );

                for ( uint32_t i = 0; i < entry.quantity(); ++i ) {
                    auto creature = std::make_unique<Engine::CreatureModel>();
                    creature->setIdCreature( _nextIdCreature++ );
                    creature->setType( entry.type() );
                    creature->position().setX( xDistribution( randomEngine ) );
                    creature->position().setY( yDistribution( randomEngine ) );
                    creature->position().setZ( z );

                    if ( creatureType ) {
                        creature->vitals().setMaxHealth( creatureType->vitals().maxHealth() );
                        creature->vitals().setHealth( creatureType->vitals().maxHealth() );
                    }

                    addCreature( std::move( creature ) );
                }
            }
        }
    }
}

void WorldRuntime::tick() {
    std::vector<Json::Value> movedCreaturePayloads;

    {
        std::lock_guard<std::mutex> lock( _mutex );

        for ( auto& entry : _characters ) {
            entry.second->tick();
        }

        for ( auto& entry : _creatures ) {
            CreatureRuntime& creatureRuntime = *entry.second;
            Engine::CreatureModel* creaturePtr = creatureRuntime.creature();

            if ( !hasCharacterNearLocked( creaturePtr->position() ) ) {
                continue;
            }

            Engine::EntityMovementModel& movement = creaturePtr->movement();
            movement.setCounter( movement.counter() + 1 );

            if ( movement.counter() < movement.cooldown() ) {
                continue;
            }

            const Engine::EntityPositionModel candidate = creatureRuntime.candidateStepPosition();
            if ( isPositionOccupiedLocked( candidate ) ) {
                continue;
            }

            creatureRuntime.commitStep();

            Json::Value payload;
            payload[ "idCreature" ] = creaturePtr->idCreature();
            payload[ "x" ] = candidate.x();
            payload[ "y" ] = candidate.y();
            payload[ "z" ] = candidate.z();

            movedCreaturePayloads.push_back( payload );
        }
    }

    for ( const Json::Value& payload : movedCreaturePayloads ) {
        _eventBus.publish( WorldEvent( WorldEventType::CREATURE_MOVED, payload ) );
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

bool WorldRuntime::hasCharacterNearLocked( const Engine::EntityPositionModel& position ) const {
    const ChunkCoordinate centerChunk = chunkCoordinateFor( position );

    for ( int dx = -1; dx <= 1; ++dx ) {
        for ( int dy = -1; dy <= 1; ++dy ) {
            const ChunkCoordinate neighborChunk( centerChunk.x + dx, centerChunk.y + dy, centerChunk.z );

            auto chunkIt = _charactersByChunk.find( neighborChunk );
            if ( chunkIt != _charactersByChunk.end() && !chunkIt->second.empty() ) {
                return true;
            }
        }
    }

    return false;
}

bool WorldRuntime::isPositionOccupiedLocked( const Engine::EntityPositionModel& position ) const {
    for ( const auto& entry : _characters ) {
        const Engine::EntityPositionModel& characterPosition = entry.second->character()->position();

        if ( characterPosition.x() == position.x() && characterPosition.y() == position.y() && characterPosition.z() == position.z() ) {
            return true;
        }
    }

    for ( const auto& entry : _creatures ) {
        const Engine::EntityPositionModel& creaturePosition = entry.second->creature()->position();

        if ( creaturePosition.x() == position.x() && creaturePosition.y() == position.y() && creaturePosition.z() == position.z() ) {
            return true;
        }
    }

    return false;
}

} // namespace Server
