#ifndef WORLDSYSTEM_H
#define WORLDSYSTEM_H

namespace Server {

class WorldRuntime;

class WorldSystem {
public:
    explicit WorldSystem( WorldRuntime& runtime );
    virtual ~WorldSystem();

    virtual void onTick() = 0;

protected:
    WorldRuntime& _runtime;
};

} // namespace Server

#endif // WORLDSYSTEM_H
