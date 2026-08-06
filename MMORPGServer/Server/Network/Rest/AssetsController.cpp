#include "AssetsController.h"

#include <Network/Filter/AuthFilter.h>

namespace Server {

void AssetsController::assetsManifest( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const {
    const NetworkSession& session = AuthFilter::session( request );

    std::cout << "AssetsController::assetsManifest [ACCOUNT] " << session.idAccount() << std::endl;

    // TODO:
    // Json::Value manifest =
    //     AssetsManager::instance().manifest();

    auto response = drogon::HttpResponse::newHttpResponse();
    response->setStatusCode( drogon::k501NotImplemented );
    callback( response );
}

void AssetsController::downloadAsset( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback, std::string path ) const {
    const NetworkSession& session = AuthFilter::session( request );

    std::cout << "AssetsController::downloadAsset [ACCOUNT] " << session.idAccount() << " [PATH] " << path << std::endl;

    // TODO:
    // AssetsManager::instance().asset(path);

    auto response = drogon::HttpResponse::newHttpResponse();
    response->setStatusCode( drogon::k501NotImplemented );
    callback( response );
}

} // namespace Server
