#include "AccountPageControl.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

#include <MMORPGClient/Client/Manager/AccountManager.h>
#include <MMORPGClient/Client/Manager/ServerManager.h>
#include <MMORPGEngine/Account/AccountCharacterDTO.h>
#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>

AccountPageControl::AccountPageControl( QObject* parent ) :
    QObject( parent ) {
}

AccountPageControl::~AccountPageControl() = default;

void AccountPageControl::logout() {
    emit logoutFailed( "" );

    ServerManager& serverManager = Engine::Singleton<ServerManager>::instance();
    if ( serverManager.connectionState() != ServerManager::ConnectionState::Connected ) {
        emit logoutFailed( tr( "Not connected to server" ) );
        return;
    }

    AccountManager& accountManager = Engine::Singleton<AccountManager>::instance();
    if ( accountManager.sessionId().isEmpty() ) {
        accountManager.clear();
        emit logoutSucceeded();
        return;
    }

    QNetworkReply* reply = serverManager.postAuthenticated( "/logout", {} );
    connect( reply, &QNetworkReply::finished, this, [ this, reply ]() {
        reply->deleteLater();

        if ( reply->error() != QNetworkReply::NoError ) {
            emit logoutFailed( reply->errorString() );
            return;
        }

        AccountManager& accountManager = Engine::Singleton<AccountManager>::instance();
        accountManager.clear();

        emit logoutSucceeded();
    } );
}

void AccountPageControl::createCharacter( const QString& name ) {
    emit characterCreationFailed( "" );

    if ( name.trimmed().isEmpty() ) {
        emit characterCreationFailed( tr( "Name is required" ) );
        return;
    }

    ServerManager& serverManager = Engine::Singleton<ServerManager>::instance();
    if ( serverManager.connectionState() != ServerManager::ConnectionState::Connected ) {
        emit characterCreationFailed( tr( "Not connected to server" ) );
        return;
    }

    QJsonObject requestJson;
    requestJson[ "name" ] = name.trimmed();

    QNetworkReply* reply = serverManager.postAuthenticated( "/create", QJsonDocument( requestJson ).toJson( QJsonDocument::Compact ) );
    connect( reply, &QNetworkReply::finished, this, [ this, reply, name ]() {
        reply->deleteLater();

        if ( reply->error() != QNetworkReply::NoError ) {
            emit characterCreationFailed( reply->errorString() );
            return;
        }

        const QByteArray responseData = reply->readAll();

        Json::Value responseJson = Engine::JsonHelper::parseJsonString( responseData.toStdString() );

        if ( responseJson.isNull() || !responseJson.isMember( "idCharacter" ) ) {
            emit characterCreationFailed( tr( "Invalid server response" ) );
            return;
        }

        Engine::AccountCharacterDTO character;
        character.setIdCharacter( responseJson[ "idCharacter" ].asInt() );
        character.setName( name.trimmed().toStdString() );

        Engine::Singleton<AccountManager>::instance().addCharacter( character );

        emit characterCreated();
    } );
}

void AccountPageControl::removeCharacter( int idCharacter ) {
    emit characterRemovalFailed( "" );

    ServerManager& serverManager = Engine::Singleton<ServerManager>::instance();
    if ( serverManager.connectionState() != ServerManager::ConnectionState::Connected ) {
        emit characterRemovalFailed( tr( "Not connected to server" ) );
        return;
    }

    QJsonObject requestJson;
    requestJson[ "characterId" ] = idCharacter;

    QNetworkReply* reply = serverManager.postAuthenticated( "/remove", QJsonDocument( requestJson ).toJson( QJsonDocument::Compact ) );
    connect( reply, &QNetworkReply::finished, this, [ this, reply, idCharacter ]() {
        reply->deleteLater();

        if ( reply->error() != QNetworkReply::NoError ) {
            emit characterRemovalFailed( reply->errorString() );
            return;
        }

        Engine::Singleton<AccountManager>::instance().removeCharacter( idCharacter );

        emit characterRemoved();
    } );
}
