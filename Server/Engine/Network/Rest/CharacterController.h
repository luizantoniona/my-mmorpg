#ifndef CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include <drogon/HttpController.h>

#include <Engine/Network/Filter/AuthFilter.h>

namespace Engine {

class CharacterController : public drogon::HttpController<CharacterController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO( CharacterController::create, "/create", drogon::Post, drogon::Options, "Network::AuthFilter" );
    ADD_METHOD_TO( CharacterController::remove, "/remove", drogon::Post, drogon::Options, "Network::AuthFilter" );
    ADD_METHOD_TO( CharacterController::list, "/characters", drogon::Get, drogon::Options, "Network::AuthFilter" );
    METHOD_LIST_END

    void create( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const;

    void remove( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const;

    void list( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const;
};

} // namespace Engine

#endif // CHARACTERCONTROLLER_H
