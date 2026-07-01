#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <string>

#include <Manager/WorldManager.h>

namespace Server {

class MessageReceiver {
public:
    MessageReceiver();

    void receive( const std::string& sessionId, const std::string& message );

private:
    WorldManager* _worldManager;
};

} // namespace Server

#endif // MESSAGERECEIVER_H
