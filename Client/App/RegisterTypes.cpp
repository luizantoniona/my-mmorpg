#include "RegisterTypes.h"

#include <QQmlEngine>

#include "Application/Account/AccountPageControl.h"
#include "Application/Game/GamePageControl.h"
#include "Application/Login/LoginPageControl.h"
#include "Manager/AccountManager.h"
#include "Manager/ServerManager.h"

void RegisterTypes::registerTypes() {
    // Page Controls
    qmlRegisterType<AccountPageControl>( "MMORPGClient.Controls", 1, 0, "AccountPageControl" );
    qmlRegisterType<GamePageControl>( "MMORPGClient.Controls", 1, 0, "GamePageControl" );
    qmlRegisterType<LoginPageControl>( "MMORPGClient.Controls", 1, 0, "LoginPageControl" );

    // Managers
    qmlRegisterType<AccountManager>( "MMORPGClient.Managers", 1, 0, "AccountManager" );
    qmlRegisterType<ServerManager>( "MMORPGClient.Managers", 1, 0, "ServerManager" );
}
