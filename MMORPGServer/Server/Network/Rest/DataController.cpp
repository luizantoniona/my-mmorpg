#include "DataController.h"

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/ManifestDTO.h>
#include <MMORPGServer/Server/Manager/DataManager.h>
#include <MMORPGServer/Server/Network/Filter/AuthFilter.h>

namespace Server {

void DataController::downloadManifest( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const {
    const NetworkSession& session = AuthFilter::session( request );

    std::cout << "DataController::downloadManifest [ACCOUNT] " << session.idAccount() << std::endl;

    const Engine::ManifestModel& manifest = Engine::Singleton<DataManager>::instance().manifest();

    Engine::ManifestDTO manifestDTO( manifest );

    auto response = drogon::HttpResponse::newHttpJsonResponse( manifestDTO.toJson() );
    response->setStatusCode( drogon::k200OK );
    callback( response );
}

void DataController::downloadData( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback, std::string path ) const {
    const NetworkSession& session = AuthFilter::session( request );

    std::cout << "DataController::downloadData [ACCOUNT] " << session.idAccount() << " [PATH] " << path << std::endl;

    // TODO:
    // DataManager::instance().data(path);

    auto response = drogon::HttpResponse::newHttpResponse();
    response->setStatusCode( drogon::k501NotImplemented );
    callback( response );
}

} // namespace Server
