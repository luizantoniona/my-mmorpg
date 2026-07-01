#include "MessageReceiver.h"

#include <Engine/Commons/JsonHelper.h>
#include <Engine/Commons/Singleton.h>

namespace Engine {

MessageReceiver::MessageReceiver() {
    _worldManager = &Singleton<WorldManager>::instance();
}

void MessageReceiver::receive( const std::string& sessionId, const std::string& message ) {
    if ( message.empty() ) {
        return;
    }

    Json::Value messageJson = JsonHelper::parseJsonString( message );
    if ( messageJson.isNull() || !messageJson.isObject() ) {
        return;
    }

    // _worldManager->handleMessage( sessionId, messageJson );
}

} // namespace Engine
