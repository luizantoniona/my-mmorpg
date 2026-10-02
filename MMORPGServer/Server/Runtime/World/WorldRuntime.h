#ifndef WORLDRUNTIME_H
#define WORLDRUNTIME_H

#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/Creature/CreatureModel.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Event/EventBus.h>
#include <MMORPGServer/Server/Runtime/Character/CharacterRuntime.h>
#include <MMORPGServer/Server/Runtime/Creature/CreatureRuntime.h>
#include <MMORPGServer/Server/Runtime/World/Command/WorldCommand.h>
#include <MMORPGServer/Server/Runtime/World/System/WorldCombatSystem.h>
#include <MMORPGServer/Server/Runtime/World/System/WorldCreatureSystem.h>
#include <MMORPGServer/Server/Runtime/World/System/WorldMovementSystem.h>
#include <MMORPGServer/Server/Runtime/World/System/WorldSpawnSystem.h>
#include <MMORPGServer/Server/Runtime/World/WorldSpatialIndex.h>

namespace Server {

class WorldRuntime {
public:
    explicit WorldRuntime( std::unique_ptr<Engine::WorldModel> world, int tickRate = 20 );

    Engine::WorldModel* world();
    const Engine::WorldModel* world() const;

    EventBus& eventBus();

    std::mutex& mutex();

    WorldSpatialIndex& spatialIndex();
    const WorldSpatialIndex& spatialIndex() const;

    WorldCombatSystem& combatSystem();
    WorldMovementSystem& movementSystem();
    WorldSpawnSystem& spawnSystem();

    int tickRate() const;

    Engine::CharacterModel* addCharacter( std::unique_ptr<Engine::CharacterModel> character );
    void removeCharacter( int idCharacter );
    Engine::CharacterModel* character( int idCharacter );

    std::map<int, Engine::EntityPositionModel> characterPositions();

    std::vector<Engine::CharacterModel> connectedCharacters();

    Engine::CreatureModel* addCreature( std::unique_ptr<Engine::CreatureModel> creature );
    Engine::CreatureModel* addCreature( std::unique_ptr<Engine::CreatureModel> creature, const WorldSpawnAreaRuntime* spawnArea );
    Engine::CreatureModel* creature( int idCreature );
    Engine::CreatureModel* creatureAt( int x, int y, int z );

    std::vector<Engine::CreatureModel> creatures();
    std::vector<Engine::CreatureModel> creaturesNear( const Engine::EntityPositionModel& position );

    Engine::CharacterModel* characterLocked( int idCharacter ) const;
    std::map<int, std::unique_ptr<CreatureRuntime>>& creaturesLocked();
    void eraseCreatureLocked( int idCreature );

    std::vector<int> charactersNear( int idCharacter );
    std::vector<int> charactersNear( const Engine::EntityPositionModel& position );

    bool isPositionOccupied( int x, int y, int z );

    void enqueueCommand( std::unique_ptr<WorldCommand> command );

    void tick();

private:
    std::mutex _mutex;
    EventBus _eventBus;
    std::unique_ptr<Engine::WorldModel> _world;
    std::map<int, std::unique_ptr<CharacterRuntime>> _characters;
    std::map<int, std::unique_ptr<CreatureRuntime>> _creatures;
    WorldSpatialIndex _spatialIndex;
    std::unique_ptr<WorldCombatSystem> _combatSystem;
    std::unique_ptr<WorldMovementSystem> _movementSystem;
    std::unique_ptr<WorldCreatureSystem> _creatureSystem;
    std::unique_ptr<WorldSpawnSystem> _spawnSystem;
    std::mutex _commandMutex;
    std::vector<std::unique_ptr<WorldCommand>> _commands;
    int _tickRate;
};

} // namespace Server

#endif // WORLDRUNTIME_H
