#include "WebSocketClient.h"

#include <QNetworkRequest>

namespace Engine {

WebSocketClient::WebSocketClient( QObject* parent ) :
    QObject( parent ) {

    connect( &_webSocket, &QWebSocket::connected, this, &WebSocketClient::connected );
    connect( &_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::disconnected );
    connect( &_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::messageReceived );
    connect( &_webSocket, &QWebSocket::errorOccurred, this, [ this ]() { emit errorOccurred( _webSocket.errorString() ); } );
}

void WebSocketClient::connectToServer( const QUrl& url, const QString& sessionId ) {
    QNetworkRequest request( url );
    request.setRawHeader( "Authorization", QString( "X-Session %1" ).arg( sessionId ).toUtf8() );

    _webSocket.open( request );
}

void WebSocketClient::disconnectFromServer() {
    _webSocket.close();
}

void WebSocketClient::sendMessage( const QString& message ) {
    _webSocket.sendTextMessage( message );
}

} // namespace Engine
