#include "ServerManager.h"

ServerManager::ServerManager( QObject* parent ) :
    QObject( parent ) {
}

ServerManager::~ServerManager() = default;

QString ServerManager::serverAddress() const {
    return _serverAddress;
}

void ServerManager::setServerAddress( const QString& serverAddress ) {
    _serverAddress = serverAddress;
}

QString ServerManager::serverPort() const {
    return _serverPort;
}

void ServerManager::setServerPort( const QString& serverPort ) {
    _serverPort = serverPort;
}
