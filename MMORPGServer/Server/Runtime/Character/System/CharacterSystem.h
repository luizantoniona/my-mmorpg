#ifndef CHARACTERSYSTEM_H
#define CHARACTERSYSTEM_H

namespace Server {

class CharacterSystem {
public:
    virtual ~CharacterSystem();

    virtual void onTick() = 0;
};

} // namespace Server

#endif // CHARACTERSYSTEM_H
