#include "LoginPageControl.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

#include <MMORPGClient/Client/Manager/AccountManager.h>
#include <MMORPGClient/Client/Manager/ServerManager.h>
#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Core/Account/AccountDTO.h>

LoginPageControl::LoginPageControl( QObject* parent ) :
    QObject( parent ) {
}

LoginPageControl::~LoginPageControl() = default;

void LoginPageControl::login( const QString& username, const QString& password ) {
    emit loginFailed( "" );

    if ( username.isEmpty() || password.isEmpty() ) {
        emit loginFailed( tr( "Username and password are required" ) );
        return;
    }

    QJsonObject requestJson;
    requestJson[ "username" ] = username;
    requestJson[ "password" ] = password;

    ServerManager& serverManager = Engine::Singleton<ServerManager>::instance();
    if ( serverManager.connectionState() != ServerManager::ConnectionState::Connected ) {
        emit loginFailed( tr( "Not connected to server" ) );
        return;
    }

    Engine::Singleton<AccountManager>::instance().clear();

    QNetworkReply* reply = serverManager.post( "/login", QJsonDocument( requestJson ).toJson( QJsonDocument::Compact ) );

    connect( reply, &QNetworkReply::finished, this, [ this, reply ]() {
        reply->deleteLater();

        if ( reply->error() != QNetworkReply::NoError ) {
            emit loginFailed( reply->errorString() );
            return;
        }

        const QByteArray responseData = reply->readAll();

        Json::Value responseJson = Engine::JsonHelper::parseJsonString( responseData.toStdString() );

        if ( responseJson.isNull() ) {
            emit loginFailed( tr( "Invalid server response" ) );
            return;
        }

        Engine::AccountDTO accountDTO = Engine::AccountDTO::fromJson( responseJson );

        if ( accountDTO.sessionId().empty() ) {
            const QString errorMessage = QString::fromStdString( responseJson[ "error" ].asString() );
            emit loginFailed( errorMessage.isEmpty() ? tr( "Login failed" ) : errorMessage );
            return;
        }

        Engine::Singleton<AccountManager>::instance().setAccount( accountDTO );

        emit loginSucceeded();
    } );
}
