#ifndef WORLDEVENTTYPE_H
#define WORLDEVENTTYPE_H

namespace Server {

enum class WorldEventType {
    ENTITY_ENTERED,
    ENTITY_MOVED,
    ENTITY_LEFT,
    ENTITY_VITALS_CHANGED,
    CREATURE_MOVED,
    CREATURE_VITALS_CHANGED,
    CREATURE_LEFT,
};

} // namespace Server

#endif // WORLDEVENTTYPE_H
