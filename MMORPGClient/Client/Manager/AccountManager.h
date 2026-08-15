#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QObject>

#include <MMORPGEngine/Account/AccountDTO.h>

class AccountManager : public QObject {
    Q_OBJECT
    Q_PROPERTY( int idAccount READ idAccount NOTIFY idAccountChanged )
    Q_PROPERTY( QString username READ username NOTIFY usernameChanged )
    Q_PROPERTY( QString sessionId READ sessionId NOTIFY sessionIdChanged )

public:
    explicit AccountManager( QObject* parent = nullptr );
    ~AccountManager();

    int idAccount() const;
    void setIdAccount( int idAccount );

    QString username() const;
    void setUsername( const QString& username );

    QString sessionId() const;
    void setSessionId( const QString& sessionId );

    void setAccount( const Engine::AccountDTO& account );
    void clear();

signals:
    void idAccountChanged();
    void usernameChanged();
    void sessionIdChanged();

private:
    int _idAccount;
    QString _username;
    QString _sessionId;
};

#endif // ACCOUNTMANAGER_H
