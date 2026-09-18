#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <atomic>
#include <thread>

namespace Server {

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    void initialize();
    void finalize();

private:
    std::atomic<bool> _running;
    std::thread _thread;
};

} // namespace Server

#endif // NETWORKMANAGER_H
