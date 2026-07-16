#include "RegisterTypes.h"

#include <QQmlEngine>

#include <MMORPGEngine/Commons/Singleton.h>

#include "Application/Account/AccountPageControl.h"
#include "Application/Game/GamePageControl.h"
#include "Application/Login/LoginPageControl.h"
#include "Manager/ServerManager.h"

void RegisterTypes::registerTypes() {
    // Page Controls
    qmlRegisterType<AccountPageControl>( "MMORPGClientControls", 1, 0, "AccountPageControl" );
    qmlRegisterType<GamePageControl>( "MMORPGClientControls", 1, 0, "GamePageControl" );
    qmlRegisterType<LoginPageControl>( "MMORPGClientControls", 1, 0, "LoginPageControl" );

    // Manager Singletons
    qmlRegisterSingletonInstance( "MMORPGClientManagers", 1, 0, "ServerManager", &Engine::Singleton<ServerManager>::instance() );
}
