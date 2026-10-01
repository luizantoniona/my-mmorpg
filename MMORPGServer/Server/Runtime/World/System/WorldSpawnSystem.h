#ifndef WORLDSPAWNSYSTEM_H
#define WORLDSPAWNSYSTEM_H

#include <MMORPGServer/Server/Runtime/World/System/WorldSystem.h>

namespace Server {

class WorldSpawnSystem : public WorldSystem {
public:
    explicit WorldSpawnSystem( WorldRuntime& runtime );

    void onTick() override;

    void spawnCreaturesFromAreas();

private:
    int _nextIdCreature;
};

} // namespace Server

#endif // WORLDSPAWNSYSTEM_H
