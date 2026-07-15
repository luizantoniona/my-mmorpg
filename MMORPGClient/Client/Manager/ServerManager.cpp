#include "ServerManager.h"

#include <QSettings>

ServerManager::ServerManager() :
    _serverAddress( "" ) {

    QSettings settings( "MyMMORPG", "Client" );
    _serverAddress = settings.value( "lastServerAddress", "http://localhost:8080" ).toString();
}

ServerManager::~ServerManager() = default;

QString ServerManager::serverAddress() const {
    return _serverAddress;
}

void ServerManager::setServerAddress( const QString& serverAddress ) {
    _serverAddress = serverAddress;

    QSettings settings( "MyMMORPG", "Client" );
    settings.setValue( "lastServerAddress", _serverAddress );
}
