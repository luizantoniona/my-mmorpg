#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>

class ServerManager : public QObject {
    Q_OBJECT
    Q_PROPERTY( QString serverAddress READ serverAddress WRITE setServerAddress NOTIFY serverAddressChanged FINAL )

public:
    explicit ServerManager( QObject* parent = nullptr );
    ~ServerManager();

    QString serverAddress() const;
    void setServerAddress( const QString& serverAddress );

signals:
    void serverAddressChanged();

private:
    QString _serverAddress;
};

#endif // SERVERMANAGER_H
