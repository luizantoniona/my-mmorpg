#include "RegisterTypes.h"

#include <QQmlEngine>

#include "Application/Account/AccountPageControl.h"
#include "Application/Game/GamePageControl.h"
#include "Application/Login/LoginPageControl.h"
#include "Manager/AccountManager.h"
#include "Manager/ServerManager.h"

void RegisterTypes::registerTypes() {
    // Page Controls
    qmlRegisterType<AccountPageControl>( "MMORPGClientControls", 1, 0, "AccountPageControl" );
    qmlRegisterType<GamePageControl>( "MMORPGClientControls", 1, 0, "GamePageControl" );
    qmlRegisterType<LoginPageControl>( "MMORPGClientControls", 1, 0, "LoginPageControl" );

    // Managers
    qmlRegisterType<AccountManager>( "MMORPGClientManagers", 1, 0, "AccountManager" );
    qmlRegisterType<ServerManager>( "MMORPGClientManagers", 1, 0, "ServerManager" );
}
