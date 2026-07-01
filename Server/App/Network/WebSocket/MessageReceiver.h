#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <string>

#include <Engine/World/WorldManager.h>

namespace Engine {

class MessageReceiver {
public:
    MessageReceiver();

    void receive( const std::string& sessionId, const std::string& message );

private:
    WorldManager* _worldManager;
};

} // namespace Engine

#endif // MESSAGERECEIVER_H
