#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QString>

class ServerManager {
public:
    ServerManager();
    ~ServerManager();

    QString serverAddress() const;
    void setServerAddress( const QString& serverAddress );

private:
    QString _serverAddress;
};

#endif // SERVERMANAGER_H
