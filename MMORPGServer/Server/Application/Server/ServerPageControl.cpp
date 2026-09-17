#include "ServerPageControl.h"

#include <QVariantMap>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>
#include <MMORPGServer/Server/Network/WebSocket/CharacterConnectionRegistry.h>

namespace Server {

ServerPageControl::ServerPageControl( QObject* parent ) :
    QObject( parent ) {
}

QString ServerPageControl::worldName() const {
    const Engine::WorldModel* world = Engine::Singleton<WorldManager>::instance().runtime().world();
    return world ? world->name() : "";
}

int ServerPageControl::worldWidth() const {
    const Engine::WorldModel* world = Engine::Singleton<WorldManager>::instance().runtime().world();
    return world ? static_cast<int>( world->width() ) : 0;
}

int ServerPageControl::worldHeight() const {
    const Engine::WorldModel* world = Engine::Singleton<WorldManager>::instance().runtime().world();
    return world ? static_cast<int>( world->height() ) : 0;
}

QVariantList ServerPageControl::floors() const {
    QVariantList result;

    const Engine::WorldModel* world = Engine::Singleton<WorldManager>::instance().runtime().world();
    if ( !world ) {
        return result;
    }

    for ( int z : world->floors() ) {
        result.append( z );
    }

    return result;
}

QVariantList ServerPageControl::connectedCharacters() const {
    QVariantList result;

    for ( const Engine::CharacterModel& character : Engine::Singleton<WorldManager>::instance().runtime().connectedCharacters() ) {
        QVariantMap entry;
        entry[ "idCharacter" ] = character.idCharacter();
        entry[ "name" ] = QString::fromStdString( character.name() );
        entry[ "x" ] = character.position().x();
        entry[ "y" ] = character.position().y();
        entry[ "z" ] = character.position().z();

        result.append( entry );
    }

    return result;
}

void ServerPageControl::disconnectCharacter( int idCharacter ) {
    drogon::WebSocketConnectionPtr connection = Engine::Singleton<CharacterConnectionRegistry>::instance().connection( idCharacter );

    if ( connection ) {
        connection->shutdown();
    }
}

} // namespace Server
