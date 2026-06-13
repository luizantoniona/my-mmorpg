#include "RegisterEngineTypes.h"

#include <QQmlEngine>

#include "Manager/AccountManager.h"
#include "Manager/ServerManager.h"

void RegisterEngineTypes::registerTypes() {
    // Managers
    qmlRegisterType<AccountManager>( "Managers", 1, 0, "AccountManager" );
    qmlRegisterType<ServerManager>( "Managers", 1, 0, "ServerManager" );
}
