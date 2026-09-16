#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Event/EventBus.h>
#include <MMORPGServer/Server/Manager/ChunkCoordinate.h>

namespace Server {

class WorldManager {
public:
    WorldManager();
    ~WorldManager();

    void initialize( const std::string& worldPath );
    void finalize();

    Engine::WorldModel* world();
    const Engine::WorldModel* world() const;

    EventBus& eventBus();

    Engine::CharacterModel* addCharacter( std::unique_ptr<Engine::CharacterModel> character );
    void removeCharacter( int idCharacter );
    Engine::CharacterModel* character( int idCharacter );

    std::map<int, Engine::EntityPositionModel> characterPositions();

    std::vector<Engine::CharacterModel> connectedCharacters();

    void moveCharacter( int idCharacter, int x, int y, int z );

    std::vector<int> charactersNear( int idCharacter );

private:
    ChunkCoordinate chunkCoordinateFor( const Engine::EntityPositionModel& position ) const;
    std::vector<int> charactersNearLocked( int idCharacter ) const;

private:
    std::atomic<bool> _running;
    std::thread _thread;
    std::unique_ptr<Engine::WorldModel> _world;
    std::mutex _mutex;
    std::map<int, std::unique_ptr<Engine::CharacterModel>> _characters;
    std::unordered_map<ChunkCoordinate, std::vector<Engine::CharacterModel*>, ChunkCoordinateHash> _charactersByChunk;
    EventBus _eventBus;
};

} // namespace Server

#endif // WORLDMANAGER_H
