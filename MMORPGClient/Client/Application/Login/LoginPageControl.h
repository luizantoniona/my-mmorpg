#ifndef LOGINPAGECONTROL_H
#define LOGINPAGECONTROL_H

#include <QNetworkAccessManager>
#include <QObject>

class LoginPageControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( QString serverAddress READ serverAddress NOTIFY serverAddressChanged FINAL )

public:
    explicit LoginPageControl( QObject* parent = nullptr );
    ~LoginPageControl();

    QString serverAddress() const;

public slots:
    void connectServer( const QString& serverAddress );

signals:
    void serverAddressChanged();
    void connectionSuccess();
    void connectionFailed();

private:
    QString _serverAddress;
    QNetworkAccessManager* _networkManager;
};

#endif // LOGINPAGECONTROL_H
