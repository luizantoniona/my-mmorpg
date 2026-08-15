#include "DataController.h"

#include <QDebug>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Manifest/ManifestDTO.h>
#include <MMORPGServer/Server/Manager/DataManager.h>
#include <MMORPGServer/Server/Network/Filter/AuthFilter.h>

namespace Server {

void DataController::downloadManifest( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const {
    const NetworkSession& session = AuthFilter::session( request );

    qInfo() << "DataController::downloadManifest [ACCOUNT] " << session.idAccount();

    const Engine::ManifestModel& manifest = Engine::Singleton<DataManager>::instance().manifest();

    Engine::ManifestDTO manifestDTO( manifest );

    auto response = drogon::HttpResponse::newHttpJsonResponse( manifestDTO.toJson() );
    response->setStatusCode( drogon::k200OK );
    callback( response );
}

void DataController::downloadData( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback, std::string path ) const {
    const NetworkSession& session = AuthFilter::session( request );

    qInfo() << "DataController::downloadData [ACCOUNT] " << session.idAccount() << " [PATH] " << path;

    // TODO: DataManager::instance().data(path);

    auto response = drogon::HttpResponse::newHttpResponse();
    response->setStatusCode( drogon::k501NotImplemented );
    callback( response );
}

} // namespace Server
