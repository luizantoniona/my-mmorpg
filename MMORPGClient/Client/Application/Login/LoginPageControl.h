#ifndef LOGINPAGECONTROL_H
#define LOGINPAGECONTROL_H

#include <QObject>

class LoginPageControl : public QObject {
    Q_OBJECT

public:
    explicit LoginPageControl( QObject* parent = nullptr );
    ~LoginPageControl();

public slots:
    void login( const QString& username, const QString& password );

signals:
    void loginSucceeded();
    void loginFailed( const QString& error );
};

#endif // LOGINPAGECONTROL_H
