#ifndef WORLDSPAWNAREARUNTIME_H
#define WORLDSPAWNAREARUNTIME_H

#include <cstdint>
#include <vector>

#include <QString>

#include <MMORPGEngine/Data/Creature/CreatureSpawnAreaModel.h>

namespace Server {

class WorldSpawnAreaRuntime {
public:
    class Pending {
    public:
        Pending( const QString& type, int64_t dueTick );

        QString type;
        int64_t dueTick;
    };

    WorldSpawnAreaRuntime( const Engine::CreatureSpawnAreaModel& area, int z );

    const Engine::CreatureSpawnAreaModel& area() const;
    int z() const;

    bool isInside( int x, int y ) const;

    bool hasPending() const;
    std::vector<Pending>& pending();
    void addPending( const QString& type, int64_t dueTick );

private:
    std::vector<Pending> _pending;
    Engine::CreatureSpawnAreaModel _area;
    int _z;
};

} // namespace Server

#endif // WORLDSPAWNAREARUNTIME_H
