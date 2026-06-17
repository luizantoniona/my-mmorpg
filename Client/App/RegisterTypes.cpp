#include "RegisterTypes.h"

#include <QQmlEngine>

#include "Application/Account/AccountPageControl.h"
#include "Application/Game/GamePageControl.h"
#include "Application/Login/LoginPageControl.h"
#include "Manager/AccountManager.h"
#include "Manager/ServerManager.h"

void RegisterTypes::registerTypes() {
    // Page Controls
    qmlRegisterType<AccountPageControl>( "Controls", 1, 0, "AccountPageControl" );
    qmlRegisterType<GamePageControl>( "Controls", 1, 0, "GamePageControl" );
    qmlRegisterType<LoginPageControl>( "Controls", 1, 0, "LoginPageControl" );

    // Managers
    qmlRegisterType<AccountManager>( "Managers", 1, 0, "AccountManager" );
    qmlRegisterType<ServerManager>( "Managers", 1, 0, "ServerManager" );
}
