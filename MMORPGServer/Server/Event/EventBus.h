#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <functional>
#include <unordered_map>
#include <vector>

#include <MMORPGServer/Server/Event/WorldEvent.h>

namespace Server {

class EventBus {
public:
    using Listener = std::function<void( const WorldEvent& )>;

    void subscribe( WorldEventType type, Listener listener );
    void publish( const WorldEvent& event );

private:
    std::unordered_map<WorldEventType, std::vector<Listener>> _listeners;
};

} // namespace Server

#endif // EVENTBUS_H
