#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include <drogon/HttpController.h>

namespace Server {

class DataController : public drogon::HttpController<DataController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO( DataController::downloadManifest, "/data/manifest", drogon::Get, drogon::Options, "Server::AuthFilter" );
    ADD_METHOD_TO( DataController::downloadData, "/data/{1:path}", drogon::Get, drogon::Options, "Server::AuthFilter" );
    METHOD_LIST_END

    void downloadManifest( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const;

    void downloadData( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback, std::string path ) const;
};

} // namespace Server

#endif // DATACONTROLLER_H
