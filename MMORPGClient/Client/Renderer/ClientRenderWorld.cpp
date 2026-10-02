#include "ClientRenderWorld.h"

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Network/WebSocket/ServerMessageReceiver.h>

ClientRenderWorld::ClientRenderWorld( QObject* parent ) :
    Engine::StreamRenderWorld( parent ),
    _world( nullptr ) {

    Engine::ServerMessageReceiver& messageReceiver = Engine::Singleton<Engine::ServerMessageReceiver>::instance();

    connect( &messageReceiver, &Engine::ServerMessageReceiver::ownCharacterReceived, this, &ClientRenderWorld::onOwnCharacterReceived );
    connect( &messageReceiver, &Engine::ServerMessageReceiver::characterStateReceived, this, &ClientRenderWorld::addCharacter );
    connect( &messageReceiver, &Engine::ServerMessageReceiver::creatureStateReceived, this, &ClientRenderWorld::addCreature );
    connect( &messageReceiver, &Engine::ServerMessageReceiver::characterEventLeaveReceived, this, &ClientRenderWorld::removeCharacter );
    connect( &messageReceiver, &Engine::ServerMessageReceiver::creatureEventLeaveReceived, this, &ClientRenderWorld::removeCreature );
}

Engine::WorldModel* ClientRenderWorld::world() const {
    return _world;
}

void ClientRenderWorld::setWorld( Engine::WorldModel* world ) {
    _world = world;

    emit boundsChanged();
}

const Engine::WorldObjectModel* ClientRenderWorld::object( int x, int y, int z ) const {
    if ( !_world ) {
        return nullptr;
    }

    return _world->object( x, y, z );
}

const Engine::WorldTileModel* ClientRenderWorld::tile( int x, int y, int z ) const {
    if ( !_world ) {
        return nullptr;
    }

    return _world->tile( x, y, z );
}

std::vector<int> ClientRenderWorld::floors() const {
    if ( !_world ) {
        return {};
    }

    return _world->floors();
}

uint32_t ClientRenderWorld::width() const {
    if ( !_world ) {
        return 0;
    }

    return _world->width();
}

uint32_t ClientRenderWorld::height() const {
    if ( !_world ) {
        return 0;
    }

    return _world->height();
}

void ClientRenderWorld::onOwnCharacterReceived( const Engine::OwnCharacterDTO& state ) {
    setOwnCharacter( state.idCharacter(), state.x(), state.y(), state.z(), state.movementCooldownSeconds() );
}
