#ifndef CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include <drogon/HttpController.h>

namespace Server {

class CharacterController : public drogon::HttpController<CharacterController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO( CharacterController::create, "/create", drogon::Post, drogon::Options, "Server::AuthFilter" );
    ADD_METHOD_TO( CharacterController::remove, "/remove", drogon::Post, drogon::Options, "Server::AuthFilter" );
    ADD_METHOD_TO( CharacterController::list, "/characters", drogon::Get, drogon::Options, "Server::AuthFilter" );
    METHOD_LIST_END

    void create( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const;

    void remove( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const;

    void list( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const;
};

} // namespace Server

#endif // CHARACTERCONTROLLER_H
