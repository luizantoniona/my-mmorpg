#include "GamePageControl.h"

#include <QUrl>

#include <MMORPGClient/Client/Manager/AccountManager.h>
#include <MMORPGClient/Client/Manager/ServerManager.h>
#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/EntityStateDTO.h>
#include <MMORPGEngine/World/WorldFactory.h>

GamePageControl::GamePageControl( QObject* parent ) :
    QObject( parent ),
    _world( nullptr ),
    _webSocket( this ),
    _idCharacter( -1 ),
    _spawnFloor( 0 ),
    _spawnX( 0 ),
    _spawnY( 0 ) {

    connect( &_webSocket, &Engine::WebSocketClient::messageReceived, this, &GamePageControl::onMessageReceived );
    connect( &_webSocket, &Engine::WebSocketClient::errorOccurred, this, &GamePageControl::worldEntryFailed );
}

GamePageControl::~GamePageControl() = default;

Engine::WorldModel* GamePageControl::world() const {
    return _world.get();
}

QVariantList GamePageControl::floors() const {
    QVariantList result;

    if ( !_world ) {
        return result;
    }

    for ( int z : _world->floors() ) {
        result.append( z );
    }

    return result;
}

QString GamePageControl::worldName() const {
    if ( !_world ) {
        return "";
    }

    return _world->name();
}

int GamePageControl::worldWidth() const {
    if ( !_world ) {
        return 0;
    }

    return static_cast<int>( _world->width() );
}

int GamePageControl::worldHeight() const {
    if ( !_world ) {
        return 0;
    }

    return static_cast<int>( _world->height() );
}

int GamePageControl::spawnFloor() const {
    return _spawnFloor;
}

int GamePageControl::spawnX() const {
    return _spawnX;
}

int GamePageControl::spawnY() const {
    return _spawnY;
}

void GamePageControl::loadWorld() {
    ServerManager& serverManager = Engine::Singleton<ServerManager>::instance();

    _world = Engine::WorldFactory::createWorld( serverManager.dataDirectory().toStdString() );

    emit worldChanged();
}

void GamePageControl::connectToWorld( int idCharacter ) {
    _idCharacter = idCharacter;

    ServerManager& serverManager = Engine::Singleton<ServerManager>::instance();

    if ( serverManager.connectionState() != ServerManager::ConnectionState::Connected ) {
        emit worldEntryFailed( tr( "Not connected to server" ) );
        return;
    }

    QUrl url( serverManager.serverAddress() );
    url.setScheme( url.scheme() == "https" ? "wss" : "ws" );
    url.setPath( "/ws/character" );
    url.setQuery( QString( "character=%1" ).arg( idCharacter ) );

    const QString sessionId = Engine::Singleton<AccountManager>::instance().sessionId();

    _webSocket.connectToServer( url, sessionId );
}

void GamePageControl::move( int dx, int dy ) {
    Json::Value json;
    json[ "type" ] = "move";
    json[ "dx" ] = dx;
    json[ "dy" ] = dy;

    _webSocket.sendMessage( QString::fromStdString( Engine::JsonHelper::writeJsonString( json ) ) );
}

void GamePageControl::onMessageReceived( const QString& message ) {
    Json::Value json = Engine::JsonHelper::parseJsonString( message.toStdString() );

    if ( json.isNull() || !json.isObject() ) {
        emit worldEntryFailed( tr( "Invalid server response" ) );
        return;
    }

    if ( json.isMember( "error" ) ) {
        emit worldEntryFailed( QString::fromStdString( json[ "error" ].asString() ) );
        return;
    }

    if ( json.get( "type", "" ).asString() == "leave" ) {
        emit entityLeftReceived( json.get( "idCharacter", -1 ).asInt() );
        return;
    }

    Engine::EntityStateDTO state = Engine::EntityStateDTO::fromJson( json );

    emit entityStateReceived( state.idCharacter(), state.x(), state.y(), state.z() );

    if ( state.idCharacter() != _idCharacter ) {
        return;
    }

    _spawnFloor = state.z();
    _spawnX = state.x();
    _spawnY = state.y();

    emit worldEntryReceived();
}
