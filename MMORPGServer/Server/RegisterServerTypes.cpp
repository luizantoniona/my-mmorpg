#include "RegisterServerTypes.h"

#include <QQmlEngine>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGServer/Server/Application/Server/ServerPageControl.h>
#include <MMORPGServer/Server/Renderer/ServerRenderWorld.h>

namespace Server {

void RegisterServerTypes::registerTypes() {
    // --- Controls
    qmlRegisterType<ServerPageControl>( "MMORPGServerComponents", 1, 0, "ServerPageControl" );

    // --- Renderer
    qmlRegisterType<ServerRenderWorld>( "MMORPGServerComponents", 1, 0, "ServerRenderWorld" );
}

} // namespace Server
