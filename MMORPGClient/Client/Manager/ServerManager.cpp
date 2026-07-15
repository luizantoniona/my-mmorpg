#include "ServerManager.h"

#include <QSettings>

namespace {
constexpr const char* SETTINGS_SCOPE = "MMORPG";
constexpr const char* SETTINGS_SUB_SCOPE = "Client";
} // namespace

ServerManager::ServerManager() :
    _serverAddress( "" ) {

    QSettings settings( QSettings::IniFormat, QSettings::UserScope, SETTINGS_SCOPE, SETTINGS_SUB_SCOPE );
    _serverAddress = settings.value( "ServerAddress", "" ).toString();
}

ServerManager::~ServerManager() = default;

QString ServerManager::serverAddress() const {
    return _serverAddress;
}

void ServerManager::setServerAddress( const QString& serverAddress ) {
    _serverAddress = serverAddress;

    QSettings settings( SETTINGS_SCOPE, SETTINGS_SUB_SCOPE );
    settings.setValue( "ServerAddress", _serverAddress );
}
