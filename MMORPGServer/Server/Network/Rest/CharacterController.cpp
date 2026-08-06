#include "CharacterController.h"

#include <Database/Database.h>
#include <Network/Filter/AuthFilter.h>
#include <Repository/CharacterRepository.h>

namespace Server {

void CharacterController::create( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const {
    const NetworkSession& session = AuthFilter::session( request );

    Json::Value body = request->getJsonObject() ? *request->getJsonObject() : Json::Value();

    if ( !body.isMember( "name" ) || !body[ "name" ].isString() ) {
        auto response = drogon::HttpResponse::newHttpJsonResponse( Json::Value{} );
        response->setStatusCode( drogon::k400BadRequest );
        return callback( response );
    }

    const std::string name = body[ "name" ].asString();

    int idAccount = session.idAccount();

    std::cout << "CharacterController::create" << " [ACCOUNT] " << idAccount << " [NAME] " << name << std::endl;

    int idCharacter = CharacterRepository().createCharacter( idAccount, name );

    if ( idCharacter == 0 ) {
        auto response = drogon::HttpResponse::newHttpResponse();
        response->setStatusCode( drogon::k500InternalServerError );
        return callback( response );
    }

    Json::Value responseJson;
    responseJson[ "message" ] = "Character created";
    responseJson[ "idCharacter" ] = idCharacter;

    auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
    response->setStatusCode( drogon::k201Created );
    callback( response );
}

void CharacterController::remove( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const {
    const NetworkSession& session = AuthFilter::session( request );

    Json::Value body = request->getJsonObject() ? *request->getJsonObject() : Json::Value();

    if ( !body.isMember( "characterId" ) || !body[ "characterId" ].isInt() ) {
        auto response = drogon::HttpResponse::newHttpJsonResponse( Json::Value{} );
        response->setStatusCode( drogon::k400BadRequest );
        return callback( response );
    }

    int idAccount = session.idAccount();
    int idCharacter = body[ "characterId" ].asInt();

    std::cout << "CharacterController::remove" << " [ACCOUNT] " << idAccount << " [CHARACTER] " << idCharacter << std::endl;

    auto character = CharacterRepository().findByIdAccountAndIdCharacter( idAccount, idCharacter );
    if ( !character || character->idAccount() != idAccount ) {
        auto response = drogon::HttpResponse::newHttpResponse();
        response->setStatusCode( drogon::k403Forbidden );
        return callback( response );
    }

    if ( !CharacterRepository().deleteCharacter( idCharacter ) ) {
        auto response = drogon::HttpResponse::newHttpResponse();
        response->setStatusCode( drogon::k500InternalServerError );
        return callback( response );
    }

    Json::Value responseJson;
    responseJson[ "message" ] = "Character removed";

    auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
    response->setStatusCode( drogon::k200OK );
    callback( response );
}

void CharacterController::list( const drogon::HttpRequestPtr& request, std::function<void( const drogon::HttpResponsePtr& )>&& callback ) const {
    const NetworkSession& session = AuthFilter::session( request );

    int idAccount = session.idAccount();

    Json::Value responseJson;
    Json::Value charactersJson( Json::arrayValue );

    auto characters = CharacterRepository().findAllByIdAccount( idAccount );

    for ( const auto& character : characters ) {

        Json::Value characterJson = ""; // character->toJson();

        // Json::Value progressionJson = character->progression().toJson();
        // for ( const auto& key : progressionJson.getMemberNames() ) {
        //     characterJson[ key ] = progressionJson[ key ];
        // }
        //
        // Json::Value stageJson = character->stage().toJson();
        // for ( const auto& key : stageJson.getMemberNames() ) {
        //     characterJson[ key ] = stageJson[ key ];
        // }
        //
        // Json::Value vitalsJson = character->vitals().toJson();
        // for ( const auto& key : vitalsJson.getMemberNames() ) {
        //     characterJson[ key ] = vitalsJson[ key ];
        // }

        charactersJson.append( characterJson );
    }

    responseJson[ "characters" ] = charactersJson;

    auto response = drogon::HttpResponse::newHttpJsonResponse( responseJson );
    response->setStatusCode( drogon::k200OK );
    callback( response );
}

} // namespace Server
