#include "ServerManager.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QUrl>

namespace {
constexpr const char* SETTINGS_SCOPE = "MMORPG";
constexpr const char* SETTINGS_SUB_SCOPE = "Client";
} // namespace

ServerManager::ServerManager( QObject* parent ) :
    QObject( parent ),
    _serverAddress( "" ),
    _connectionState( ConnectionState::Disconnected ),
    _networkManager() {

    QSettings settings( QSettings::IniFormat, QSettings::UserScope, SETTINGS_SCOPE, SETTINGS_SUB_SCOPE );
    _serverAddress = settings.value( "ServerAddress", "" ).toString();

    connectServer( _serverAddress );
}

ServerManager::~ServerManager() = default;

QString ServerManager::serverAddress() const {
    return _serverAddress;
}

void ServerManager::setServerAddress( const QString& address ) {
    if ( _serverAddress == address ) {
        return;
    }

    _serverAddress = address;

    QSettings settings( QSettings::IniFormat, QSettings::UserScope, SETTINGS_SCOPE, SETTINGS_SUB_SCOPE );
    settings.setValue( "ServerAddress", address );

    emit serverAddressChanged();
}

ServerManager::ConnectionState ServerManager::connectionState() const {
    return _connectionState;
}

void ServerManager::setConnectionState( ConnectionState state ) {
    if ( _connectionState == state ) {
        return;
    }

    _connectionState = state;

    emit connectionStateChanged();
}

void ServerManager::connectServer( const QString& address ) {
    if ( _connectionState == ConnectionState::Connecting ) {
        return;
    }

    setConnectionState( ConnectionState::Connecting );

    QString formatted = address;
    if ( !formatted.startsWith( "http://" ) && !formatted.startsWith( "https://" ) ) {
        formatted.prepend( "http://" );
    }

    auto reply = _networkManager.get( QNetworkRequest( QUrl( formatted + "/status" ) ) );

    connect( reply, &QNetworkReply::finished, this, [ this, reply ]() {
        QString finalAddress = reply->request().url().toString().replace( "/status", "" );

        reply->deleteLater();

        if ( reply->error() == QNetworkReply::NoError ) {
            setServerAddress( finalAddress );
            setConnectionState( ConnectionState::Connected );

        } else {
            setConnectionState( ConnectionState::Failed );
        }
    } );
}
