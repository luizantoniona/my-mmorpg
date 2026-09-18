#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QUrl>
#include <QWebSocket>

namespace Engine {

class WebSocketClient : public QObject {
    Q_OBJECT

public:
    explicit WebSocketClient( QObject* parent = nullptr );

    void connectToServer( const QUrl& url, const QString& sessionId );
    void disconnectFromServer();

    void sendMessage( const QString& message );

signals:
    void connected();
    void disconnected();
    void messageReceived( const QString& message );
    void errorOccurred( const QString& error );

private:
    QWebSocket _webSocket;
};

} // namespace Engine

#endif // WEBSOCKETCLIENT_H
