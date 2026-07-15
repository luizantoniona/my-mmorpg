#include "LoginPageControl.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QUrl>

#include <MMORPGEngine/Commons/Singleton.h>
#include <Manager/ServerManager.h>

LoginPageControl::LoginPageControl( QObject* parent ) :
    QObject( parent ),
    _networkManager( new QNetworkAccessManager( this ) ) {

    _serverAddress = Engine::Singleton<ServerManager>::instance().serverAddress();
}

LoginPageControl::~LoginPageControl() = default;

QString LoginPageControl::serverAddress() const {
    return _serverAddress;
}

void LoginPageControl::connectServer( const QString& serverAddress ) {
    QString formattedAddress = serverAddress;
    if ( !formattedAddress.startsWith( "http://" ) && !formattedAddress.startsWith( "https://" ) ) {
        formattedAddress = "http://" + formattedAddress;
    }

    QUrl url( formattedAddress + "/status" );
    QNetworkRequest request( url );

    QNetworkReply* reply = _networkManager->get( request );

    connect( reply, &QNetworkReply::finished, this, [ this, reply, formattedAddress ]() {
        reply->deleteLater();

        if ( reply->error() == QNetworkReply::NoError ) {
            QString finalAddress = reply->request().url().toString().replace( "/status", "" );

            _serverAddress = finalAddress;

            emit serverAddressChanged();

            emit connectionSuccess();

            Engine::Singleton<ServerManager>::instance().setServerAddress( finalAddress );

        } else {
            emit connectionFailed();
        }
    } );
}
