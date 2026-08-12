#include "AccountPageControl.h"

#include <QNetworkReply>

#include <MMORPGClient/Client/Manager/AccountManager.h>
#include <MMORPGClient/Client/Manager/ServerManager.h>
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
