#include "ServerManager.h"

#include <QSettings>

#include <MMORPGClient/Client/Manager/AccountManager.h>
#include <MMORPGEngine/Commons/Singleton.h>

namespace {
constexpr const char* SETTINGS_SCOPE = "MMORPG";
constexpr const char* SETTINGS_SUB_SCOPE = "Client";
} // namespace

ServerManager::ServerManager( QObject* parent ) :
    QObject( parent ),
    _connectionState( ConnectionState::Disconnected ),
    _httpClient( this ) {

    QSettings settings( QSettings::IniFormat, QSettings::UserScope, SETTINGS_SCOPE, SETTINGS_SUB_SCOPE );
    _serverAddress = settings.value( "ServerAddress", "" ).toUrl();

    connectServer( _serverAddress.toString() );
}

ServerManager::~ServerManager() = default;

QString ServerManager::serverAddress() const {
    return _serverAddress.toString();
}

ServerManager::ConnectionState ServerManager::connectionState() const {
    return _connectionState;
}

QNetworkReply* ServerManager::get( const QString& endpoint ) {
    return _httpClient.get( endpoint );
}

QNetworkReply* ServerManager::getAuthenticated( const QString& endpoint ) {
    AccountManager& accountManager = Engine::Singleton<AccountManager>::instance();
    return _httpClient.getAuthenticated( endpoint, accountManager.sessionId() );
}

QNetworkReply* ServerManager::post( const QString& endpoint, const QByteArray& body ) {
    return _httpClient.post( endpoint, body );
}

QNetworkReply* ServerManager::postAuthenticated( const QString& endpoint, const QByteArray& body ) {
    AccountManager& accountManager = Engine::Singleton<AccountManager>::instance();
    return _httpClient.postAuthenticated( endpoint, body, accountManager.sessionId() );
}

void ServerManager::disconnectServer() {
    if ( _connectionState == ConnectionState::Disconnected && _serverAddress.isEmpty() ) {
        return;
    }

    // Futuramente:
    // _httpClient.abortAllRequests();
    // _webSocket.disconnectFromHost();

    setServerAddress( {} );
    setConnectionState( ConnectionState::Disconnected );
}

void ServerManager::connectServer( const QString& address ) {
    if ( _connectionState == ConnectionState::Connecting ) {
        return;
    }

    if ( address.isEmpty() ) {
        return;
    }

    setConnectionState( ConnectionState::Connecting );

    QString url = address.trimmed();

    if ( !url.startsWith( "http://" ) && !url.startsWith( "https://" ) ) {
        url.prepend( "http://" );
    }

    QUrl serverUrl( url );

    _httpClient.setBaseUrl( serverUrl );

    auto reply = _httpClient.get( "/status" );
    connect( reply, &QNetworkReply::finished, this, [ this, reply, serverUrl ]() {
        reply->deleteLater();

        if ( reply->error() == QNetworkReply::NoError ) {
            setServerAddress( serverUrl );
            setConnectionState( ConnectionState::Connected );

        } else {
            setConnectionState( ConnectionState::Failed );
        }
    } );
}

void ServerManager::setServerAddress( const QUrl& serverAddress ) {
    if ( _serverAddress == serverAddress ) {
        return;
    }

    _serverAddress = serverAddress;

    _httpClient.setBaseUrl( serverAddress );

    QSettings settings( QSettings::IniFormat, QSettings::UserScope, SETTINGS_SCOPE, SETTINGS_SUB_SCOPE );
    settings.setValue( "ServerAddress", serverAddress );

    emit serverAddressChanged();
}

void ServerManager::setConnectionState( ConnectionState connectionState ) {
    if ( _connectionState == connectionState ) {
        return;
    }

    _connectionState = connectionState;

    emit connectionStateChanged();
}
