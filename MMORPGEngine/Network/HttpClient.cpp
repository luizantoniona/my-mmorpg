#include "HttpClient.h"

namespace Engine {

HttpClient::HttpClient( QObject* parent ) :
    QObject( parent ) {
}

void HttpClient::setBaseUrl( const QUrl& url ) {
    _baseUrl = url;
}

QUrl HttpClient::baseUrl() const {
    return _baseUrl;
}

QNetworkReply* HttpClient::get( const QString& endpoint, const QHash<QByteArray, QByteArray>& extraHeaders ) {
    return _networkManager.get( buildRequest( buildUrl( endpoint ), extraHeaders ) );
}

QNetworkReply* HttpClient::post( const QString& endpoint, const QByteArray& body, const QHash<QByteArray, QByteArray>& extraHeaders ) {
    return _networkManager.post( buildRequest( buildUrl( endpoint ), extraHeaders ), body );
}

QUrl HttpClient::buildUrl( const QString& endpoint ) const {
    QUrl url = _baseUrl;

    QString path = url.path();

    if ( !path.endsWith( '/' ) ) {
        path += '/';
    }

    path += endpoint.startsWith( '/' ) ? endpoint.mid( 1 ) : endpoint;

    url.setPath( path );

    return url;
}

QNetworkRequest HttpClient::buildRequest( const QUrl& url, const QHash<QByteArray, QByteArray>& headers ) const {
    QNetworkRequest request( url );

    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
    request.setRawHeader( "Accept", "application/json" );

    for ( auto it = headers.begin(); it != headers.end(); ++it ) {
        request.setRawHeader( it.key(), it.value() );
    }

    return request;
}

} // namespace Engine
