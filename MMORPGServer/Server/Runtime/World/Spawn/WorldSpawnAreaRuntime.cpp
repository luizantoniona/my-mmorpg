#include "WorldSpawnAreaRuntime.h"

namespace Server {

WorldSpawnAreaRuntime::Pending::Pending( uint32_t type, int64_t dueTick ) :
    type( type ),
    dueTick( dueTick ) {
}

WorldSpawnAreaRuntime::WorldSpawnAreaRuntime( const Engine::CreatureSpawnAreaModel& area, int z ) :
    _pending(),
    _area( area ),
    _z( z ) {
}

const Engine::CreatureSpawnAreaModel& WorldSpawnAreaRuntime::area() const {
    return _area;
}

int WorldSpawnAreaRuntime::z() const {
    return _z;
}

bool WorldSpawnAreaRuntime::isInside( int x, int y ) const {
    return x >= _area.x() && x < _area.x() + static_cast<int>( _area.width() ) && y >= _area.y() && y < _area.y() + static_cast<int>( _area.height() );
}

bool WorldSpawnAreaRuntime::hasPending() const {
    return !_pending.empty();
}

std::vector<WorldSpawnAreaRuntime::Pending>& WorldSpawnAreaRuntime::pending() {
    return _pending;
}

void WorldSpawnAreaRuntime::addPending( uint32_t type, int64_t dueTick ) {
    _pending.emplace_back( type, dueTick );
}

} // namespace Server
