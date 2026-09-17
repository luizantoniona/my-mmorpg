#include "WorldEvent.h"

namespace Server {

WorldEvent::WorldEvent( WorldEventType type, const Json::Value& payload ) :
    _payload( payload ),
    _type( type ) {
}

WorldEventType WorldEvent::type() const {
    return _type;
}

const Json::Value& WorldEvent::payload() const {
    return _payload;
}

} // namespace Server
