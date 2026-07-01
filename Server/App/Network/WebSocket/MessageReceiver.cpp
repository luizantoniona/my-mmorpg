#include "MessageReceiver.h"

#include <Engine/Commons/JsonHelper.h>
#include <Engine/Commons/Singleton.h>

namespace Server {

MessageReceiver::MessageReceiver() {
    _worldManager = &Engine::Singleton<WorldManager>::instance();
}

void MessageReceiver::receive( const std::string& sessionId, const std::string& message ) {
    if ( message.empty() ) {
        return;
    }

    Json::Value messageJson = Engine::JsonHelper::parseJsonString( message );
    if ( messageJson.isNull() || !messageJson.isObject() ) {
        return;
    }

    // _worldManager->handleMessage( sessionId, messageJson );
}

} // namespace Server
