#include "RegisterServerTypes.h"

#include <QQmlEngine>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGServer/Server/Application/Server/ServerRenderWorld.h>

namespace Server {

void RegisterServerTypes::registerTypes() {
    qmlRegisterType<ServerRenderWorld>( "MMORPGServerComponents", 1, 0, "ServerRenderWorld" );
}

} // namespace Server
