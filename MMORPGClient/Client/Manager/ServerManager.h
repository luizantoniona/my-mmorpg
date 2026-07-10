#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>

class ServerManager : public QObject {
    Q_OBJECT

public:
    explicit ServerManager( QObject* parent = nullptr );
    ~ServerManager();

    QString serverAddress() const;
    void setServerAddress( const QString& serverAddress );

    QString serverPort() const;
    void setServerPort( const QString& serverPort );

private:
    QString _serverAddress;
    QString _serverPort;
};

#endif // SERVERMANAGER_H
