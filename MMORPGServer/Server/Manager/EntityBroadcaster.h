#ifndef ENTITYBROADCASTER_H
#define ENTITYBROADCASTER_H

#include <MMORPGServer/Server/Event/WorldEvent.h>

namespace Server {

class EntityBroadcaster {
public:
    EntityBroadcaster();

private:
    void onEntityEntered( const WorldEvent& event );
    void onEntityMoved( const WorldEvent& event );
    void onEntityLeft( const WorldEvent& event );

    void broadcastPosition( const WorldEvent& event );
    void broadcastVitals( const WorldEvent& event );
};

} // namespace Server

#endif // ENTITYBROADCASTER_H
