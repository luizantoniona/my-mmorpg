#ifndef CHUNKRUNTIME_H
#define CHUNKRUNTIME_H

#include <vector>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGServer/Server/Manager/ChunkCoordinate.h>
#include <MMORPGServer/Server/Runtime/Creature/CreatureRuntime.h>
#include <MMORPGServer/Server/Runtime/World/Spawn/WorldSpawnAreaRuntime.h>

namespace Server {

class ChunkRuntime {
public:
    explicit ChunkRuntime( const ChunkCoordinate& coordinate );

    const ChunkCoordinate& coordinate() const;

    std::vector<Engine::CharacterModel*>& characters();
    const std::vector<Engine::CharacterModel*>& characters() const;

    std::vector<CreatureRuntime*>& creatures();
    const std::vector<CreatureRuntime*>& creatures() const;

    std::vector<int>& groundItems();
    const std::vector<int>& groundItems() const;

    std::vector<WorldSpawnAreaRuntime*>& spawnAreas();
    const std::vector<WorldSpawnAreaRuntime*>& spawnAreas() const;

    void addObserver();
    void removeObserver();

    bool hasObservers() const;
    bool hasPendingRespawn() const;
    bool isActive() const;

private:
    std::vector<Engine::CharacterModel*> _characters;
    std::vector<CreatureRuntime*> _creatures;
    std::vector<int> _groundItems;
    std::vector<WorldSpawnAreaRuntime*> _spawnAreas;
    ChunkCoordinate _coordinate;
    int _observerCount;
};

} // namespace Server

#endif // CHUNKRUNTIME_H
