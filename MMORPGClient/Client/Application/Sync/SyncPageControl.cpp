#include "SyncPageControl.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QNetworkReply>

#include <MMORPGClient/Client/Manager/ServerManager.h>
#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Manifest/ManifestDTO.h>
#include <MMORPGEngine/Data/Manifest/ManifestFactory.h>

SyncPageControl::SyncPageControl( QObject* parent ) :
    QObject( parent ),
    _downloadedCount( 0 ),
    _totalCount( 0 ),
    _progress( 0.0 ) {
}

SyncPageControl::~SyncPageControl() = default;

qreal SyncPageControl::progress() const {
    return _progress;
}

void SyncPageControl::setProgress( qreal progress ) {
    _progress = progress;

    emit progressChanged();
}

void SyncPageControl::sync() {
    setProgress( 0.0 );
    emit syncUpdate( tr( "Downloading manifest..." ) );

    ServerManager& serverManager = Engine::Singleton<ServerManager>::instance();

    if ( serverManager.connectionState() != ServerManager::ConnectionState::Connected ) {
        emit syncFailed( tr( "Not connected to server" ) );
        return;
    }

    QNetworkReply* reply = serverManager.getAuthenticated( "/data/manifest" );

    connect( reply, &QNetworkReply::finished, this, [ this, reply ]() {
        reply->deleteLater();

        if ( reply->error() != QNetworkReply::NoError ) {
            emit syncFailed( reply->errorString() );
            return;
        }

        const QByteArray responseData = reply->readAll();

        Json::Value responseJson = Engine::JsonHelper::parseJsonString( responseData.toStdString() );

        if ( responseJson.isNull() || !responseJson.isObject() ) {
            emit syncFailed( tr( "Invalid server response" ) );
            return;
        }

        Engine::ManifestDTO manifest = Engine::ManifestDTO::fromJson( responseJson );

        if ( !manifest.isValid() ) {
            emit syncFailed( tr( "Invalid manifest" ) );
            return;
        }

        _downloadQueue.clear();

        const QString dataDirectory = Engine::Singleton<ServerManager>::instance().dataDirectory();

        for ( const Engine::ManifestDataDTO& data : manifest.data() ) {
            const QString localPath = dataDirectory + QString::fromStdString( data.path() );

            if ( QFileInfo::exists( localPath ) && Engine::ManifestFactory::hashFile( localPath ) == data.hash() ) {
                continue;
            }

            _downloadQueue.append( data );
        }

        _downloadedCount = 0;
        _totalCount = _downloadQueue.size();

        downloadNext();
    } );
}

void SyncPageControl::downloadNext() {
    ServerManager& serverManager = Engine::Singleton<ServerManager>::instance();

    if ( _downloadQueue.isEmpty() ) {
        setProgress( 1.0 );

        Engine::Singleton<Engine::DataManager>::instance().reload( serverManager.dataDirectory().toStdString() );

        emit syncSucceeded();
        return;
    }

    const Engine::ManifestDataDTO data = _downloadQueue.takeFirst();
    const QString relativePath = QString::fromStdString( data.path() );

    emit syncUpdate( tr( "Downloading %1 (%2/%3)..." ).arg( relativePath ).arg( _downloadedCount + 1 ).arg( _totalCount ) );

    QNetworkReply* reply = serverManager.getAuthenticated( "/data/" + relativePath );

    connect( reply, &QNetworkReply::finished, this, [ this, reply, relativePath ]() {
        reply->deleteLater();

        if ( reply->error() != QNetworkReply::NoError ) {
            emit syncFailed( reply->errorString() );
            return;
        }

        const QString localPath = Engine::Singleton<ServerManager>::instance().dataDirectory() + relativePath;

        QDir().mkpath( QFileInfo( localPath ).absolutePath() );

        QFile file( localPath );
        if ( !file.open( QIODevice::WriteOnly ) ) {
            emit syncFailed( tr( "Failed to write %1" ).arg( localPath ) );
            return;
        }

        file.write( reply->readAll() );
        file.close();

        _downloadedCount++;
        setProgress( _totalCount > 0 ? qreal( _downloadedCount ) / _totalCount : 1.0 );

        downloadNext();
    } );
}
