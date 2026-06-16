#include "CharacterController.h"

#include <optional>

#include <Engine/Character/CharacterRepository.h>
#include <Engine/Commons/Singleton.h>
#include <Engine/Database/Database.h>
#include <Engine/Network/NetworkServer.h>
#include <Engine/Network/NetworkSession.h>

namespace Engine {

void CharacterController::create( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const {
    auto token = request->getHeader( "Authorization" );
    const std::string prefix = "X-Session ";

    if ( token.rfind( prefix, 0 ) != 0 ) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode( drogon::k401Unauthorized );
        return callback( resp );
    }

    std::string sessionId = token.substr( prefix.length() );
    std::optional<NetworkSession> session = Singleton<NetworkServer>::instance().getSession( sessionId );

    if ( !session ) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode( drogon::k401Unauthorized );
        return callback( resp );
    }

    Json::Value body = request->getJsonObject() ? *request->getJsonObject() : Json::Value();

    if ( !body.isMember( "name" ) || !body[ "name" ].isString() ) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse( Json::Value{} );
        resp->setStatusCode( drogon::k400BadRequest );
        return callback( resp );
    }

    const std::string name = body[ "name" ].asString();

    int idAccount = session->idAccount();

    std::cout << "CharacterController::create" << " [ID] " << idAccount << " [NAME] " << name << std::endl;

    int idCharacter = CharacterRepository().createCharacter( idAccount, name );

    if ( idCharacter == 0 ) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode( drogon::k500InternalServerError );
        return callback( resp );
    }

    Json::Value responseJson;
    responseJson[ "message" ] = "Character created";
    responseJson[ "idCharacter" ] = idCharacter;

    auto resp = drogon::HttpResponse::newHttpJsonResponse( responseJson );
    resp->setStatusCode( drogon::k201Created );
    callback( resp );
}

void CharacterController::remove( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const {
    auto token = request->getHeader( "Authorization" );
    const std::string prefix = "X-Session ";

    if ( token.rfind( prefix, 0 ) != 0 ) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode( drogon::k401Unauthorized );
        return callback( resp );
    }

    std::string sessionId = token.substr( prefix.length() );
    std::optional<NetworkSession> session = Singleton<NetworkServer>::instance().getSession( sessionId );

    if ( !session ) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode( drogon::k401Unauthorized );
        return callback( resp );
    }

    Json::Value body = request->getJsonObject() ? *request->getJsonObject() : Json::Value();

    if ( !body.isMember( "characterId" ) || !body[ "characterId" ].isInt() ) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse( Json::Value{} );
        resp->setStatusCode( drogon::k400BadRequest );
        return callback( resp );
    }

    int idAccount = session->idAccount();
    int idCharacter = body[ "characterId" ].asInt();

    std::cout << "CharacterController::remove" << " [ACCOUNT] " << idAccount << " [CHARACTER] " << idCharacter << std::endl;

    auto character = CharacterRepository().findByIdAccountAndIdCharacter( idAccount, idCharacter );
    if ( !character || character->idAccount() != idAccount ) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode( drogon::k403Forbidden );
        return callback( resp );
    }

    bool success = CharacterRepository().deleteCharacter( idCharacter );

    if ( !success ) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode( drogon::k500InternalServerError );
        return callback( resp );
    }

    Json::Value responseJson;
    responseJson[ "message" ] = "Character removed";

    auto resp = drogon::HttpResponse::newHttpJsonResponse( responseJson );
    resp->setStatusCode( drogon::k200OK );
    callback( resp );
}

void CharacterController::list( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const {
    auto token = request->getHeader( "Authorization" );
    const std::string prefix = "X-Session ";

    if ( token.rfind( prefix, 0 ) == 0 ) {
        std::string sessionId = token.substr( prefix.length() );
        std::optional<NetworkSession> session = Singleton<NetworkServer>::instance().getSession( sessionId );

        if ( session ) {
            int idAccount = session->idAccount();

            Json::Value responseJson;
            Json::Value charactersJson( Json::arrayValue );

            auto characters = CharacterRepository().findAllByIdAccount( idAccount );

            for ( const auto& character : characters ) {
                Json::Value characterJson = ""; // character->toJson();

                // Json::Value progressionJson = character->progression().toJson();
                // for ( const auto& key : progressionJson.getMemberNames() ) {
                //     characterJson[ key ] = progressionJson[ key ];
                // }

                // Json::Value stageJson = character->stage().toJson();
                // for ( const auto& key : stageJson.getMemberNames() ) {
                //     characterJson[ key ] = stageJson[ key ];
                // }

                // Json::Value vitalsJson = character->vitals().toJson();
                // for ( const auto& key : vitalsJson.getMemberNames() ) {
                //     characterJson[ key ] = vitalsJson[ key ];
                // }

                charactersJson.append( characterJson );
            }

            responseJson[ "characters" ] = charactersJson;
            auto resp = drogon::HttpResponse::newHttpJsonResponse( responseJson );
            resp->setStatusCode( drogon::k200OK );
            return callback( resp );
        }
    }

    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setStatusCode( drogon::k401Unauthorized );
    callback( resp );
}

} // namespace Engine
