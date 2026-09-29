#ifndef CLIENTMESSAGETYPEENUM_H
#define CLIENTMESSAGETYPEENUM_H

namespace Engine {

enum class ClientMessageType {
    UNKNOWN,
    CHARACTER_INTENT_MOVE,
    CHARACTER_INTENT_ATTACK,
};

} // namespace Engine

#endif // CLIENTMESSAGETYPEENUM_H
