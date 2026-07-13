#include "ServerManager.h"

ServerManager::ServerManager( QObject* parent ) :
    QObject( parent ),
    _serverAddress( "" ) {}

ServerManager::~ServerManager() = default;

QString ServerManager::serverAddress() const {
    return _serverAddress;
}

void ServerManager::setServerAddress( const QString& serverAddress ) {
    if ( _serverAddress == serverAddress ) {
        return;
    }

    _serverAddress = serverAddress;

    emit serverAddressChanged();
}
