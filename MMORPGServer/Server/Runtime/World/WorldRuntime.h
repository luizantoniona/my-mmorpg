#ifndef WORLDRUNTIME_H
#define WORLDRUNTIME_H

#include <map>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/Creature/CreatureModel.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Event/EventBus.h>
#include <MMORPGServer/Server/Manager/ChunkCoordinate.h>
#include <MMORPGServer/Server/Runtime/Character/CharacterRuntime.h>
#include <MMORPGServer/Server/Runtime/Creature/CreatureRuntime.h>

namespace Server {

class WorldRuntime {
public:
    explicit WorldRuntime( std::unique_ptr<Engine::WorldModel> world );

    Engine::WorldModel* world();
    const Engine::WorldModel* world() const;

    EventBus& eventBus();

    Engine::CharacterModel* addCharacter( std::unique_ptr<Engine::CharacterModel> character );
    void removeCharacter( int idCharacter );
    Engine::CharacterModel* character( int idCharacter );

    std::map<int, Engine::EntityPositionModel> characterPositions();

    std::vector<Engine::CharacterModel> connectedCharacters();

    Engine::CreatureModel* addCreature( std::unique_ptr<Engine::CreatureModel> creature );
    std::vector<Engine::CreatureModel> creatures();

    void spawnCreaturesFromAreas();

    void moveCharacter( int idCharacter, int x, int y, int z );

    std::vector<int> charactersNear( int idCharacter );

    bool isPositionOccupied( int x, int y, int z );
    bool isCharacterMoveDue( int idCharacter );

    void tick();

private:
    ChunkCoordinate chunkCoordinateFor( const Engine::EntityPositionModel& position ) const;
    std::vector<int> charactersNearLocked( int idCharacter ) const;
    bool hasCharacterNearLocked( const Engine::EntityPositionModel& position ) const;
    bool isPositionOccupiedLocked( const Engine::EntityPositionModel& position ) const;

private:
    std::mutex _mutex;
    std::unique_ptr<Engine::WorldModel> _world;
    std::map<int, std::unique_ptr<CharacterRuntime>> _characters;
    std::unordered_map<ChunkCoordinate, std::vector<Engine::CharacterModel*>, ChunkCoordinateHash> _charactersByChunk;
    std::map<int, std::unique_ptr<CreatureRuntime>> _creatures;
    EventBus _eventBus;
    int _nextIdCreature;
};

} // namespace Server

#endif // WORLDRUNTIME_H
