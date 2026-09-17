#ifndef NETWORKMESSAGETYPEENUM_H
#define NETWORKMESSAGETYPEENUM_H

namespace Engine {

enum class NetworkMessageType {
    UNKNOWN,
    MOVE,
    OWN_CHARACTER,
    CHARACTER,
    CREATURE_STATE,
    ENTITY_LEFT,
};

} // namespace Engine

#endif // NETWORKMESSAGETYPEENUM_H
