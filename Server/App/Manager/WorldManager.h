#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <memory>
#include <string>
#include <thread>

#include <Engine/World/WorldModel.h>

namespace Server {

class WorldManager {
public:
    WorldManager();
    ~WorldManager();

    void initialize( const std::string& worldPath );
    void finalize();

private:
    std::atomic<bool> _running;
    std::thread _thread;
    std::unique_ptr<Engine::WorldModel> _world;
};

} // namespace Server

#endif // WORLDMANAGER_H
