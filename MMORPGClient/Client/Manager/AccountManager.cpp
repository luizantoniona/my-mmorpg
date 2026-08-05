#include "AccountManager.h"

AccountManager::AccountManager( QObject* parent ) :
    QObject( parent ),
    _idAccount( 0 ),
    _username( "" ),
    _sessionId( "" ) {
}

AccountManager::~AccountManager() = default;

int AccountManager::idAccount() const {
    return _idAccount;
}

void AccountManager::setIdAccount( int idAccount ) {
    if ( _idAccount == idAccount ) {
        return;
    }

    _idAccount = idAccount;

    emit idAccountChanged();
}

QString AccountManager::username() const {
    return _username;
}

void AccountManager::setUsername( const QString& username ) {
    if ( _username == username ) {
        return;
    }

    _username = username;

    emit usernameChanged();
}

QString AccountManager::sessionId() const {
    return _sessionId;
}

void AccountManager::setSessionId( const QString& sessionId ) {
    if ( _sessionId == sessionId ) {
        return;
    }

    _sessionId = sessionId;

    emit sessionIdChanged();
}

void AccountManager::setAccount( const Engine::AccountDTO& account ) {
    setIdAccount( account.idAccount() );
    setUsername( QString::fromStdString( account.username() ) );
    setSessionId( QString::fromStdString( account.sessionId() ) );
}

void AccountManager::clear() {
    setIdAccount( 0 );
    setUsername( "" );
    setSessionId( "" );
}
