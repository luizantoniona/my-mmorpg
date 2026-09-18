#ifndef WORLDSYSTEM_H
#define WORLDSYSTEM_H

namespace Server {

class WorldSystem {
public:
    virtual ~WorldSystem();

    virtual void onTick() = 0;
};

} // namespace Server

#endif // WORLDSYSTEM_H
