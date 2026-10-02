#ifndef OBSERVERREGISTRY_H
#define OBSERVERREGISTRY_H

#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include <MMORPGServer/Server/Network/Observer/EntityObserver.h>

namespace Server {

class ObserverRegistry {
public:
    ObserverRegistry();

    void registerCharacterObserver( int idCharacter, const std::shared_ptr<EntityObserver>& observer );
    void unregisterCharacterObserver( int idCharacter );

    void registerGlobalObserver( const std::shared_ptr<EntityObserver>& observer );
    void unregisterGlobalObserver( const std::shared_ptr<EntityObserver>& observer );

    std::shared_ptr<EntityObserver> characterObserver( int idCharacter );
    std::vector<std::shared_ptr<EntityObserver>> globalObservers();

private:
    std::mutex _mutex;
    std::unordered_map<int, std::shared_ptr<EntityObserver>> _characterObservers;
    std::vector<std::shared_ptr<EntityObserver>> _globalObservers;
};

} // namespace Server

#endif // OBSERVERREGISTRY_H
