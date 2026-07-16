#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QNetworkAccessManager>
#include <QObject>

class ServerManager : public QObject {
    Q_OBJECT
    Q_PROPERTY( QString serverAddress READ serverAddress NOTIFY serverAddressChanged )
    Q_PROPERTY( ConnectionState connectionState READ connectionState NOTIFY connectionStateChanged )

public:
    explicit ServerManager( QObject* parent = nullptr );
    ~ServerManager();

    enum class ConnectionState {
        Disconnected,
        Connected,
        Connecting,
        Failed
    };
    Q_ENUM( ConnectionState )

    QString serverAddress() const;

    ConnectionState connectionState() const;

    QNetworkReply* get( const QString& endpoint );
    QNetworkReply* post( const QString& endpoint, const QByteArray& body );

public slots:
    void connectServer( const QString& address );

signals:
    void serverAddressChanged();
    void connectionStateChanged();

private:
    QUrl buildUrl( const QString& endpoint ) const;
    QNetworkRequest buildRequest( const QString& endpoint ) const;

    void setServerAddress( const QUrl& serverAddress );
    void setConnectionState( ConnectionState connectionState );

private:
    QUrl _serverAddress;
    ConnectionState _connectionState;
    QNetworkAccessManager _networkManager;
};
#endif // SERVERMANAGER_H
