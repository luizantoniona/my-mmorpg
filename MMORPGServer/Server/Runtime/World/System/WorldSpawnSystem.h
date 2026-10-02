#ifndef WORLDSPAWNSYSTEM_H
#define WORLDSPAWNSYSTEM_H

#include <cstdint>
#include <map>
#include <memory>
#include <random>
#include <vector>

#include <MMORPGServer/Server/Event/WorldEvent.h>
#include <MMORPGServer/Server/Runtime/World/Spawn/WorldSpawnAreaRuntime.h>
#include <MMORPGServer/Server/Runtime/World/System/WorldSystem.h>

namespace Server {

class WorldSpawnSystem : public WorldSystem {
public:
    explicit WorldSpawnSystem( WorldRuntime& runtime );

    void onTick() override;

    const std::vector<std::unique_ptr<WorldSpawnAreaRuntime>>& areas() const;
    size_t activeAreaCount() const;

private:
    void buildAreas();
    void onCreatureDied( const WorldEvent& event );

    void spawnDue( WorldSpawnAreaRuntime& area );
    bool spawnCreature( WorldSpawnAreaRuntime& area, uint32_t type );

    int64_t ticksFromSeconds( double seconds ) const;

private:
    std::vector<std::unique_ptr<WorldSpawnAreaRuntime>> _areas;
    std::vector<WorldSpawnAreaRuntime*> _activeAreas;
    std::map<int, WorldSpawnAreaRuntime*> _creatureAreas;
    std::mt19937 _randomEngine;
    int64_t _currentTick;
    int _nextIdCreature;
    bool _areasBuilt;
};

} // namespace Server

#endif // WORLDSPAWNSYSTEM_H
