#ifndef WORLDEVENT_H
#define WORLDEVENT_H

#include <json/json.h>

#include <MMORPGServer/Server/Event/WorldEventType.h>

namespace Server {

class WorldEvent {
public:
    WorldEvent( WorldEventType type, const Json::Value& payload );

    WorldEventType type() const;
    const Json::Value& payload() const;

private:
    Json::Value _payload;
    WorldEventType _type;
};

} // namespace Server

#endif // WORLDEVENT_H
