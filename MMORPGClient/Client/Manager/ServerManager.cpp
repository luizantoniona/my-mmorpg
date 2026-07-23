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
    return _networkManager.get( buildRequest( endpoint ) );
}

QNetworkReply* ServerManager::post( const QString& endpoint, const QByteArray& body ) {
    return _networkManager.post( buildRequest( endpoint ), body );
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

    QNetworkRequest request( serverUrl.resolved( QUrl( "/status" ) ) );

    auto reply = _networkManager.get( request );

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

QUrl ServerManager::buildUrl( const QString& endpoint ) const {
    QUrl url = _serverAddress;

    QString path = url.path();

    if ( !path.endsWith( '/' ) ) {
        path += '/';
    }

    path += endpoint.startsWith( '/' ) ? endpoint.mid( 1 ) : endpoint;

    url.setPath( path );

    return url;
}

QNetworkRequest ServerManager::buildRequest( const QString& endpoint ) const {
    QNetworkRequest request( buildUrl( endpoint ) );

    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    request.setRawHeader( "Accept", "application/json" );

    return request;
}

void ServerManager::setServerAddress( const QUrl& serverAddress ) {
    if ( _serverAddress == serverAddress ) {
        return;
    }

    _serverAddress = serverAddress;

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

/*
Engine::Singleton<ServerManager>

ServerManager
├── HTTP
│   ├── GET
│   └── POST
├── WebSocket
│   ├── connect()
│   ├── disconnect()
│   ├── send()
│   └── receive()
├── Session
│   ├── token
│   ├── refresh token
│   └── headers
├── Server
│   ├── address
│   ├── ping
│   └── status
└── Settings
    └── salvar endereço
*/
