#ifndef ASSETSCONTROLLER_H
#define ASSETSCONTROLLER_H

#include <drogon/HttpController.h>

namespace Server {

class AssetsController : public drogon::HttpController<AssetsController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO( AssetsController::assetsManifest, "/assets", drogon::Get, drogon::Options, "Server::AuthFilter" );
    ADD_METHOD_TO( AssetsController::downloadAsset, "/assets/{1:path}", drogon::Get, drogon::Options, "Server::AuthFilter" );
    METHOD_LIST_END

    void assetsManifest( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const;

    void downloadAsset( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback, std::string path ) const;
};

} // namespace Server

#endif // ASSETSCONTROLLER_H
