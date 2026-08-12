#include "SyncPageControl.h"

#include <QNetworkReply>

#include <MMORPGClient/Client/Manager/ServerManager.h>
#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/ManifestDTO.h>

SyncPageControl::SyncPageControl( QObject* parent ) :
    QObject( parent ) {
}

SyncPageControl::~SyncPageControl() = default;

void SyncPageControl::sync() {
    emit syncUpdate( tr( "Downloading manifest..." ) );

    ServerManager& serverManager = Engine::Singleton<ServerManager>::instance();

    if ( serverManager.connectionState() != ServerManager::ConnectionState::Connected ) {
        emit syncFailed( tr( "Not connected to server" ) );
        return;
    }

    QNetworkReply* reply = serverManager.getAuthenticated( "/data/manifest" );

    connect( reply, &QNetworkReply::finished, this, [ this, reply ]() {
        reply->deleteLater();

        if ( reply->error() != QNetworkReply::NoError ) {
            emit syncFailed( reply->errorString() );
            return;
        }

        const QByteArray responseData = reply->readAll();

        Json::Value responseJson = Engine::JsonHelper::parseJsonString( responseData.toStdString() );

        if ( responseJson.isNull() || !responseJson.isObject() ) {
            emit syncFailed( tr( "Invalid server response" ) );
            return;
        }

        Engine::ManifestDTO manifest = Engine::ManifestDTO::fromJson( responseJson );

        if ( !manifest.isValid() ) {
            emit syncFailed( tr( "Invalid manifest" ) );
            return;
        }

        qInfo() << "SyncPageControl::sync" << "Manifest received";

        emit syncSucceeded();
    } );
}
