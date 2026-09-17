#include "DataController.h"

#include <QDebug>
#include <QFileInfo>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Manifest/ManifestDTO.h>
#include <MMORPGServer/Server/Network/Filter/AuthFilter.h>

namespace Server {

void DataController::downloadManifest( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const {
    const NetworkSession& session = AuthFilter::session( request );

    qInfo() << "DataController::downloadManifest [ACCOUNT] " << session.idAccount();

    const Engine::ManifestModel manifest = Engine::Singleton<Engine::DataManager>::instance().manifest();

    Engine::ManifestDTO manifestDTO( manifest );

    auto response = drogon::HttpResponse::newHttpJsonResponse( manifestDTO.toJson() );
    response->setStatusCode( drogon::k200OK );
    callback( response );
}

void DataController::downloadData( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback, std::string path ) const {
    const NetworkSession& session = AuthFilter::session( request );

    qInfo() << "DataController::downloadData [ACCOUNT] " << session.idAccount() << " [PATH] " << path;

    const std::string& configPath = Engine::Singleton<Engine::DataManager>::instance().configPath();

    const QFileInfo rootInfo( QString::fromStdString( configPath ) );
    const QFileInfo fileInfo( QString::fromStdString( configPath + path ) );

    if ( !fileInfo.exists() || !fileInfo.canonicalFilePath().startsWith( rootInfo.canonicalFilePath() ) ) {
        auto response = drogon::HttpResponse::newHttpResponse();
        response->setStatusCode( drogon::k404NotFound );
        callback( response );
        return;
    }

    auto response = drogon::HttpResponse::newFileResponse( fileInfo.canonicalFilePath().toStdString() );
    callback( response );
}

} // namespace Server
