#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <memory>
#include <string>
#include <thread>

#include "WorldModel.h"

namespace Engine {

class WorldManager {
public:
    WorldManager();
    ~WorldManager();

    void initialize( const std::string& worldPath );
    void finalize();

private:
    std::atomic<bool> _running;
    std::thread _thread;
    std::unique_ptr<WorldModel> _world;
};

} // namespace Engine

#endif // WORLDMANAGER_H
