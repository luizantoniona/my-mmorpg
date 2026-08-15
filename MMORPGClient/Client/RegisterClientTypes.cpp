#include "RegisterClientTypes.h"

#include <QQmlEngine>

#include <MMORPGClient/Client/Application/Account/AccountPageControl.h>
#include <MMORPGClient/Client/Application/Game/GamePageControl.h>
#include <MMORPGClient/Client/Application/Login/LoginPageControl.h>
#include <MMORPGClient/Client/Application/Sync/SyncPageControl.h>
#include <MMORPGClient/Client/Manager/ServerManager.h>
#include <MMORPGEngine/Commons/Singleton.h>

namespace Client {

void RegisterClientTypes::registerTypes() {
    // Page Controls
    qmlRegisterType<AccountPageControl>( "MMORPGClientControls", 1, 0, "AccountPageControl" );
    qmlRegisterType<GamePageControl>( "MMORPGClientControls", 1, 0, "GamePageControl" );
    qmlRegisterType<LoginPageControl>( "MMORPGClientControls", 1, 0, "LoginPageControl" );
    qmlRegisterType<SyncPageControl>( "MMORPGClientControls", 1, 0, "SyncPageControl" );

    // Manager Singletons
    qmlRegisterSingletonInstance( "MMORPGClientManagers", 1, 0, "ServerManager", &Engine::Singleton<ServerManager>::instance() );
}

} // namespace Client
