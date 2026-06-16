#include "AuthController.h"

#include <Engine/Account/AccountRepository.h>
#include <Engine/Commons/Singleton.h>
#include <Engine/Database/Database.h>
#include <Engine/Network/NetworkServer.h>

namespace Engine {

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

    std::string sessionId = Singleton<NetworkServer>::instance().createSession( account->idAccount(), account->dsUsername() );

    responseJson[ "idAccount" ] = account->idAccount();
    responseJson[ "username" ] = account->dsUsername();
    responseJson[ "sessionID" ] = sessionId;
    responseJson[ "message" ] = "Login successful";

    std::cout << "AuthController::login [Account] " << account->dsUsername() << " [UUID] " << sessionId << std::endl;

    auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
    response->setStatusCode( drogon::k200OK );
    callback( response );
}

void AuthController::logout( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const {
    auto token = request->getHeader( "Authorization" );
    const std::string prefix = "X-Session ";

    Json::Value responseJson;

    if ( token.rfind( prefix, 0 ) == 0 ) {
        std::string sessionId = token.substr( prefix.length() );
        std::optional<NetworkSession> session = Singleton<NetworkServer>::instance().getSession( sessionId );

        if ( session && Singleton<NetworkServer>::instance().deleteSession( sessionId ) ) {

            std::cout << "AuthController::logout" << " [UUID] " << sessionId << std::endl;

            responseJson[ "message" ] = "Logout successful";
            auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
            response->setStatusCode( drogon::k200OK );
            callback( response );
            return;

        } else {
            responseJson[ "error" ] = "Failed to destroy session";
            auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
            response->setStatusCode( drogon::k500InternalServerError );
            callback( response );
            return;
        }
    }

    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setStatusCode( drogon::k401Unauthorized );
    callback( resp );
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

    std::string sessionId = Singleton<NetworkServer>::instance().createSession( account->idAccount(), account->dsUsername() );

    responseJson[ "idAccount" ] = account->idAccount();
    responseJson[ "username" ] = account->dsUsername();
    responseJson[ "sessionID" ] = sessionId;
    responseJson[ "message" ] = "Account created successfully";

    std::cout << "AuthController::sign [Account] " << account->dsUsername() << " [UUID] " << sessionId << std::endl;

    auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
    response->setStatusCode( drogon::k201Created );
    callback( response );
}

} // namespace Engine
