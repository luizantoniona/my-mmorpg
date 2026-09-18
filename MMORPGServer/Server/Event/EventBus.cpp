#include "EventBus.h"

namespace Server {

void EventBus::subscribe( WorldEventType type, Listener listener ) {
    _listeners[ type ].push_back( listener );
}

void EventBus::publish( const WorldEvent& event ) {
    auto it = _listeners.find( event.type() );
    if ( it == _listeners.end() ) {
        return;
    }

    for ( auto& listener : it->second ) {
        listener( event );
    }
}

} // namespace Server
