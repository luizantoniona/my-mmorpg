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
    void onEntityVitalsChanged( const WorldEvent& event );
    void onCreatureMoved( const WorldEvent& event );
    void onCreatureVitalsChanged( const WorldEvent& event );
    void onCreatureLeft( const WorldEvent& event );

    void sendWorldBasic( const WorldEvent& event );
    void sendOwnCharacter( const WorldEvent& event );
    void sendOwnEquipment( const WorldEvent& event );
    void sendOwnInventory( const WorldEvent& event );
    void sendNearbyCharacters( const WorldEvent& event );
    void sendCreatures( const WorldEvent& event );
    void broadcastCharacter( const WorldEvent& event );
    void broadcastCreature( int idCreature );
};

} // namespace Server

#endif // ENTITYBROADCASTER_H
