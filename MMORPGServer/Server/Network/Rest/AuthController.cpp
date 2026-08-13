#include "AuthController.h"

#include <MMORPGEngine/Account/AccountDTO.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGServer/Server/Database/Database.h>
#include <MMORPGServer/Server/Network/Filter/AuthFilter.h>
#include <MMORPGServer/Server/Network/NetworkServer.h>
#include <MMORPGServer/Server/Repository/AccountRepository.h>

namespace Server {

void AuthController::login( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const {
    auto requestJson = request->getJsonObject();

    Json::Value responseJson;

    if ( !requestJson ) {
        responseJson[ "error" ] = "No JSON";
        auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
        response->setStatusCode( drogon::k400BadRequest );
        callback( response );
        return;
    }

    if ( !requestJson->isMember( "username" ) || !requestJson->isMember( "password" ) ) {
        responseJson[ "error" ] = "Invalid JSON";
        auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
        response->setStatusCode( drogon::k400BadRequest );
        callback( response );
        return;
    }

    std::string username = ( *requestJson )[ "username" ].asString();
    std::string password = ( *requestJson )[ "password" ].asString();

    auto account = AccountRepository().findByUsernameAndPassword( username, password );

    if ( !account ) {
        responseJson[ "error" ] = "Invalid credentials";
        auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
        response->setStatusCode( drogon::k401Unauthorized );
        callback( response );
        return;
    }

    std::string sessionId = Engine::Singleton<NetworkServer>::instance().createSession( account->idAccount(), account->dsUsername() );

    Engine::AccountDTO accountDTO( *account, sessionId );

    std::cout << "AuthController::login [Account] " << account->dsUsername() << " [UUID] " << sessionId << std::endl;

    auto response = drogon::HttpResponse::newHttpJsonResponse( accountDTO.toJson() );
    response->setStatusCode( drogon::k200OK );
    callback( response );
}

void AuthController::logout( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const {
    const NetworkSession& session = AuthFilter::session( request );

    Json::Value responseJson;

    if ( !Engine::Singleton<NetworkServer>::instance().deleteSession( session.uuid() ) ) {
        responseJson[ "error" ] = "Failed to destroy session";

        auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
        response->setStatusCode( drogon::k500InternalServerError );

        callback( response );
        return;
    }

    std::cout << "AuthController::logout [UUID] "
              << session.uuid()
              << std::endl;

    auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
    response->setStatusCode( drogon::k200OK );

    callback( response );
}

void AuthController::sign( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const {
    auto requestJson = request->getJsonObject();
    Json::Value responseJson;

    if ( !requestJson ) {
        responseJson[ "error" ] = "No JSON";
        auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
        response->setStatusCode( drogon::k400BadRequest );
        callback( response );
        return;
    }

    if ( !requestJson->isMember( "username" ) || !requestJson->isMember( "password" ) ) {
        responseJson[ "error" ] = "Invalid JSON";
        auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
        response->setStatusCode( drogon::k400BadRequest );
        callback( response );
        return;
    }

    std::string username = ( *requestJson )[ "username" ].asString();
    std::string password = ( *requestJson )[ "password" ].asString();

    if ( username.empty() || password.empty() ) {
        responseJson[ "error" ] = "Username or password empty";
        auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
        response->setStatusCode( drogon::k400BadRequest );
        callback( response );
        return;
    }

    AccountRepository accountRepository;

    if ( accountRepository.findByUsername( username ) ) {
        responseJson[ "error" ] = "Failed to create account";
        auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
        response->setStatusCode( drogon::k409Conflict );
        callback( response );
        return;
    }

    if ( !accountRepository.createAccount( username, password ) ) {
        responseJson[ "error" ] = "Failed to create account";
        auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
        response->setStatusCode( drogon::k500InternalServerError );
        callback( response );
        return;
    }

    auto account = accountRepository.findByUsername( username );

    if ( !account ) {
        responseJson[ "error" ] = "Account created but not found";
        auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
        response->setStatusCode( drogon::k500InternalServerError );
        callback( response );
        return;
    }

    std::string sessionId = Engine::Singleton<NetworkServer>::instance().createSession( account->idAccount(), account->dsUsername() );

    Engine::AccountDTO accountDTO( *account, sessionId );

    std::cout << "AuthController::sign [Account] " << account->dsUsername() << " [UUID] " << sessionId << std::endl;

    auto response = drogon::HttpResponse::newHttpJsonResponse( accountDTO.toJson() );
    response->setStatusCode( drogon::k201Created );
    callback( response );
}

} // namespace Server
