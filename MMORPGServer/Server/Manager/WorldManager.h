#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <atomic>
#include <memory>
#include <string>
#include <thread>

#include <MMORPGServer/Server/Runtime/WorldRuntime.h>

namespace Server {

class WorldManager {
public:
    WorldManager();
    ~WorldManager();

    void initialize( const std::string& worldPath );
    void finalize();

    WorldRuntime& runtime();

private:
    std::atomic<bool> _running;
    std::thread _thread;
    std::unique_ptr<WorldRuntime> _runtime;
};

} // namespace Server

#endif // WORLDMANAGER_H
