#ifndef ENTITYBROADCASTER_H
#define ENTITYBROADCASTER_H

#include <string>
#include <vector>

#include <MMORPGServer/Server/Event/WorldEvent.h>
#include <MMORPGServer/Server/Network/Observer/EntityObserver.h>

namespace Server {

class EntityBroadcaster {
public:
    EntityBroadcaster();

    void sendSnapshot( EntityObserver& observer );

private:
    void onEntityEntered( const WorldEvent& event );
    void onEntityMoved( const WorldEvent& event );
    void onEntityLeft( const WorldEvent& event );
    void onEntityVitalsChanged( const WorldEvent& event );
    void onEntityAttacked( const WorldEvent& event );
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
    void broadcastAttack( const WorldEvent& event );
    void broadcastCreature( int idCreature );

    void sendToCharacter( int idCharacter, const std::string& message );
    void broadcast( const std::vector<int>& idCharacters, const std::string& message );
    void broadcastToConnected( const std::string& message );
};

} // namespace Server

#endif // ENTITYBROADCASTER_H
