#ifndef WORLDSPAWNAREARUNTIME_H
#define WORLDSPAWNAREARUNTIME_H

#include <cstdint>
#include <vector>

#include <MMORPGEngine/Data/Creature/CreatureSpawnAreaModel.h>

namespace Server {

class WorldSpawnAreaRuntime {
public:
    class Pending {
    public:
        Pending( uint32_t type, int64_t dueTick );

        uint32_t type;
        int64_t dueTick;
    };

    WorldSpawnAreaRuntime( const Engine::CreatureSpawnAreaModel& area, int z );

    const Engine::CreatureSpawnAreaModel& area() const;
    int z() const;

    bool isInside( int x, int y ) const;

    bool hasPending() const;
    std::vector<Pending>& pending();
    void addPending( uint32_t type, int64_t dueTick );

private:
    std::vector<Pending> _pending;
    Engine::CreatureSpawnAreaModel _area;
    int _z;
};

} // namespace Server

#endif // WORLDSPAWNAREARUNTIME_H
