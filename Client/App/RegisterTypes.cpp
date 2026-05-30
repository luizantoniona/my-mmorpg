#include "RegisterTypes.h"

#include <QQmlEngine>

#include "Manager/AccountManager.h"

void RegisterTypes::registerTypes() {

    // Managers
    qmlRegisterType<AccountManager>( "Managers", 1, 0, "AccountManager" );
}
