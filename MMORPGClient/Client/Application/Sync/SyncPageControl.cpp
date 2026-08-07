#include "SyncPageControl.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

#include <MMORPGClient/Client/Manager/ServerManager.h>
#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>

SyncPageControl::SyncPageControl( QObject* parent ) :
    QObject( parent ) {
}

SyncPageControl::~SyncPageControl() = default;

void SyncPageControl::sync() {
    emit syncUpdate( "Downloading assets..." );

    ServerManager& serverManager = Engine::Singleton<ServerManager>::instance();
    if ( serverManager.connectionState() != ServerManager::ConnectionState::Connected ) {
        emit syncFailed( tr( "Not connected to server" ) );
        return;
    }

    QNetworkReply* reply = serverManager.get( "/assets" );

    connect( reply, &QNetworkReply::finished, this, [ this, reply ]() {
        reply->deleteLater();

        if ( reply->error() != QNetworkReply::NoError ) {
            emit syncFailed( reply->errorString() );
            return;
        }

        const QByteArray responseData = reply->readAll();

        Json::Value responseJson = Engine::JsonHelper::parseJsonString( responseData.toStdString() );

        if ( responseJson.isNull() ) {
            emit syncFailed( tr( "Invalid server response" ) );
            return;
        }

        emit syncSucceeded();
    } );
}
