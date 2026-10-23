#ifndef WORLDVISIBILITYSYSTEM_H
#define WORLDVISIBILITYSYSTEM_H

#include <vector>

#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGServer/Server/Manager/ChunkCoordinate.h>
#include <MMORPGServer/Server/Runtime/World/System/WorldSystem.h>
#include <MMORPGServer/Server/Runtime/World/WorldVisibilityDelta.h>

namespace Server {

class WorldVisibilitySystem : public WorldSystem {
public:
    explicit WorldVisibilitySystem( WorldRuntime& runtime );

    void onTick() override;

    WorldVisibilityDelta delta( const Engine::EntityPositionModel& previousPosition, const Engine::EntityPositionModel& currentPosition, int ignoredIdCharacter );

    std::vector<ChunkCoordinate> terrainChunksNear( const Engine::EntityPositionModel& position ) const;
};

} // namespace Server

#endif // WORLDVISIBILITYSYSTEM_H
