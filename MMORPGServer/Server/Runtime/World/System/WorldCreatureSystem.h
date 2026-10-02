#ifndef WORLDCREATURESYSTEM_H
#define WORLDCREATURESYSTEM_H

#include <MMORPGServer/Server/Runtime/World/System/WorldSystem.h>

namespace Server {

class WorldCreatureSystem : public WorldSystem {
public:
    explicit WorldCreatureSystem( WorldRuntime& runtime );

    void onTick() override;
};

} // namespace Server

#endif // WORLDCREATURESYSTEM_H
