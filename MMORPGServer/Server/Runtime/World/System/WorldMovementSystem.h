#ifndef WORLDMOVEMENTSYSTEM_H
#define WORLDMOVEMENTSYSTEM_H

#include <MMORPGServer/Server/Runtime/World/System/WorldSystem.h>

namespace Server {

class WorldMovementSystem : public WorldSystem {
public:
    explicit WorldMovementSystem( WorldRuntime& runtime );

    void onTick() override;

    bool move( int idCharacter, int dx, int dy );
    void moveCharacter( int idCharacter, int x, int y, int z );
};

} // namespace Server

#endif // WORLDMOVEMENTSYSTEM_H
