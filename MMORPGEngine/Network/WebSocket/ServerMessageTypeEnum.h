#ifndef SERVERMESSAGETYPEENUM_H
#define SERVERMESSAGETYPEENUM_H

namespace Engine {

enum class ServerMessageType {
    UNKNOWN,
    WORLD_BASIC,
    OWN_CHARACTER,
    OWN_EQUIPMENT,
    OWN_INVENTORY,
    CHARACTER,
    CREATURE,
    ENTITY_LEFT,
};

} // namespace Engine

#endif // SERVERMESSAGETYPEENUM_H
