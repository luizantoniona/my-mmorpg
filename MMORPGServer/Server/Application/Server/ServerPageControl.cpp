#include "ServerPageControl.h"

#include <QVariantMap>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Manager/EntityBroadcaster.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>
#include <MMORPGServer/Server/Network/Observer/ObserverRegistry.h>

namespace Server {

ServerPageControl::ServerPageControl( QObject* parent ) :
    QObject( parent ),
    _godObserver( std::make_shared<GodObserver>() ),
    _messageReceiver( this ) {

    connect( _godObserver.get(), &GodObserver::messageReceived, &_messageReceiver, &Engine::ServerMessageReceiver::receiveMessage );

    connect( &_messageReceiver, &Engine::ServerMessageReceiver::characterStateReceived, this, &ServerPageControl::characterStateReceived );
    connect( &_messageReceiver, &Engine::ServerMessageReceiver::creatureStateReceived, this, &ServerPageControl::creatureStateReceived );
    connect( &_messageReceiver, &Engine::ServerMessageReceiver::entityAttackedReceived, this, &ServerPageControl::entityAttackedReceived );
    connect( &_messageReceiver, &Engine::ServerMessageReceiver::entityLeftReceived, this, &ServerPageControl::entityLeftReceived );
    connect( &_messageReceiver, &Engine::ServerMessageReceiver::creatureLeftReceived, this, &ServerPageControl::creatureLeftReceived );

    Engine::Singleton<ObserverRegistry>::instance().registerGlobalObserver( _godObserver );

    Engine::Singleton<EntityBroadcaster>::instance().sendSnapshot( *_godObserver );
}

ServerPageControl::~ServerPageControl() {
    Engine::Singleton<ObserverRegistry>::instance().unregisterGlobalObserver( _godObserver );
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
    std::shared_ptr<EntityObserver> observer = Engine::Singleton<ObserverRegistry>::instance().characterObserver( idCharacter );

    if ( observer ) {
        observer->shutdown();
    }
}

} // namespace Server
