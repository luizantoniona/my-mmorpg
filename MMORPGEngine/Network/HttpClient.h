#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>

namespace Engine {

class HttpClient : public QObject {
    Q_OBJECT

public:
    explicit HttpClient( QObject* parent = nullptr );

    void setBaseUrl( const QUrl& url );
    QUrl baseUrl() const;

    QNetworkReply* get( const QString& endpoint, const QHash<QByteArray, QByteArray>& extraHeaders = {} );
    QNetworkReply* getAuthenticated( const QString& endpoint, const QString& sessionId, const QHash<QByteArray, QByteArray>& extraHeaders = {} );

    QNetworkReply* post( const QString& endpoint, const QByteArray& body, const QHash<QByteArray, QByteArray>& extraHeaders = {} );
    QNetworkReply* postAuthenticated( const QString& endpoint, const QByteArray& body, const QString& sessionId, const QHash<QByteArray, QByteArray>& extraHeaders = {} );

private:
    QUrl buildUrl( const QString& endpoint ) const;
    QNetworkRequest buildRequest( const QUrl& url, const QHash<QByteArray, QByteArray>& headers ) const;

private:
    QUrl _baseUrl;
    QNetworkAccessManager _networkManager;
};

} // namespace Engine

#endif // HTTPCLIENT_H
