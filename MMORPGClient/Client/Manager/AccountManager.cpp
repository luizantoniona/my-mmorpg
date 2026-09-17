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

QVariantList AccountManager::characters() const {
    QVariantList result;

    for ( const Engine::AccountCharacterDTO& character : _characters ) {
        QVariantMap characterMap;
        characterMap[ "idCharacter" ] = character.idCharacter();
        characterMap[ "name" ] = QString::fromStdString( character.name() );

        result.append( characterMap );
    }

    return result;
}

void AccountManager::setCharacters( const std::vector<Engine::AccountCharacterDTO>& characters ) {
    _characters = characters;

    emit charactersChanged();
}

void AccountManager::addCharacter( const Engine::AccountCharacterDTO& character ) {
    _characters.push_back( character );

    emit charactersChanged();
}

void AccountManager::removeCharacter( int idCharacter ) {
    std::erase_if( _characters, [ idCharacter ]( const Engine::AccountCharacterDTO& character ) {
        return character.idCharacter() == idCharacter;
    } );

    emit charactersChanged();
}

void AccountManager::setAccount( const Engine::AccountDTO& account ) {
    setIdAccount( account.idAccount() );
    setUsername( QString::fromStdString( account.username() ) );
    setSessionId( QString::fromStdString( account.sessionId() ) );
    setCharacters( account.characters() );
}

void AccountManager::clear() {
    setIdAccount( 0 );
    setUsername( "" );
    setSessionId( "" );
    setCharacters( {} );
}
