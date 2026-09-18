#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <vector>

#include <QObject>
#include <QVariantList>

#include <MMORPGEngine/Account/AccountCharacterDTO.h>
#include <MMORPGEngine/Account/AccountDTO.h>

class AccountManager : public QObject {
    Q_OBJECT
    Q_PROPERTY( int idAccount READ idAccount NOTIFY idAccountChanged )
    Q_PROPERTY( QString username READ username NOTIFY usernameChanged )
    Q_PROPERTY( QString sessionId READ sessionId NOTIFY sessionIdChanged )
    Q_PROPERTY( QVariantList characters READ characters NOTIFY charactersChanged )

public:
    explicit AccountManager( QObject* parent = nullptr );
    ~AccountManager();

    int idAccount() const;
    void setIdAccount( int idAccount );

    QString username() const;
    void setUsername( const QString& username );

    QString sessionId() const;
    void setSessionId( const QString& sessionId );

    QVariantList characters() const;
    void setCharacters( const std::vector<Engine::AccountCharacterDTO>& characters );
    void addCharacter( const Engine::AccountCharacterDTO& character );
    void removeCharacter( int idCharacter );

    void setAccount( const Engine::AccountDTO& account );
    void clear();

signals:
    void idAccountChanged();
    void usernameChanged();
    void sessionIdChanged();
    void charactersChanged();

private:
    int _idAccount;
    QString _username;
    QString _sessionId;
    std::vector<Engine::AccountCharacterDTO> _characters;
};

#endif // ACCOUNTMANAGER_H
