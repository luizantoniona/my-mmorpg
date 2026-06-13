#include "RegisterTypes.h"

#include <QQmlEngine>

#include "Application/Account/AccountPageControl.h"
#include "Application/Game/GamePageControl.h"
#include "Application/Login/LoginPageControl.h"

void RegisterTypes::registerTypes() {
    // Page Controls
    qmlRegisterType<AccountPageControl>( "Controls", 1, 0, "AccountPageControl" );
    qmlRegisterType<GamePageControl>( "Controls", 1, 0, "GamePageControl" );
    qmlRegisterType<LoginPageControl>( "Controls", 1, 0, "LoginPageControl" );
}
