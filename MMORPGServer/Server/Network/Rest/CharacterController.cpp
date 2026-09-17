#include "CharacterController.h"

#include <QDebug>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/Entity/EntityVitalsModel.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Database/Database.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>
#include <MMORPGServer/Server/Network/Filter/AuthFilter.h>
#include <MMORPGServer/Server/Repository/Character/CharacterRepository.h>

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

    qInfo() << "CharacterController::create"
            << " [ACCOUNT] " << idAccount << " [NAME] " << name;

    const Engine::WorldModel* world = Engine::Singleton<WorldManager>::instance().runtime().world();

    Engine::EntityPositionModel spawnPosition;
    spawnPosition.setX( world ? world->spawnX() : 0 );
    spawnPosition.setY( world ? world->spawnY() : 0 );
    spawnPosition.setZ( world ? world->spawnZ() : 0 );

    // TODO: Derive from the attribute system once it exists; flat baseline for now.
    Engine::EntityVitalsModel spawnVitals;
    spawnVitals.setHealth( 100.0 );
    spawnVitals.setMaxHealth( 100.0 );
    spawnVitals.setMana( 50.0 );
    spawnVitals.setMaxMana( 50.0 );
    spawnVitals.setStamina( 50.0 );
    spawnVitals.setMaxStamina( 50.0 );

    int idCharacter = CharacterRepository().createCharacter( idAccount, name, spawnPosition, spawnVitals );

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

    qInfo() << "CharacterController::remove" << " [ACCOUNT] " << idAccount << " [CHARACTER] " << idCharacter;

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

} // namespace Server
