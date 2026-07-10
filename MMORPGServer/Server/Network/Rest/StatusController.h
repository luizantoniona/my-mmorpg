#ifndef STATUSCONTROLLER_H
#define STATUSCONTROLLER_H

#include <drogon/drogon.h>

namespace Engine {

class StatusController : public drogon::HttpController<StatusController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO( StatusController::status, "/status", drogon::Get, drogon::Options );
    METHOD_LIST_END

    void status( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const;
};

} // namespace Engine

#endif // STATUSCONTROLLER_H
