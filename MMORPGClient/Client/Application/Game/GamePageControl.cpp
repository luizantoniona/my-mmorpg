#include "GamePageControl.h"

#include <QUrl>

#include <MMORPGClient/Client/Manager/AccountManager.h>
#include <MMORPGClient/Client/Manager/ServerManager.h>
#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/EntityOrientationModel.h>
#include <MMORPGEngine/Entity/EntityStateDTO.h>
#include <MMORPGEngine/Entity/EntityVitalsDTO.h>
#include <MMORPGEngine/Entity/EntityVitalsModel.h>
#include <MMORPGEngine/World/WorldFactory.h>

GamePageControl::GamePageControl( QObject* parent ) :
    QObject( parent ),
    _world( nullptr ),
    _webSocket( this ),
    _character() {

    _character.setIdCharacter( -1 );

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
    return _character.position().z();
}

int GamePageControl::spawnX() const {
    return _character.position().x();
}

int GamePageControl::spawnY() const {
    return _character.position().y();
}

double GamePageControl::health() const {
    return _character.vitals().health();
}

double GamePageControl::maxHealth() const {
    return _character.vitals().maxHealth();
}

double GamePageControl::mana() const {
    return _character.vitals().mana();
}

double GamePageControl::maxMana() const {
    return _character.vitals().maxMana();
}

double GamePageControl::stamina() const {
    return _character.vitals().stamina();
}

double GamePageControl::maxStamina() const {
    return _character.vitals().maxStamina();
}

void GamePageControl::loadWorld() {
    ServerManager& serverManager = Engine::Singleton<ServerManager>::instance();

    _world = Engine::WorldFactory::createWorld( serverManager.dataDirectory().toStdString() );

    emit worldChanged();
}

void GamePageControl::connectToWorld( int idCharacter ) {
    _character.setIdCharacter( idCharacter );

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

    if ( json.get( "type", "" ).asString() == "vitals" ) {
        Engine::EntityVitalsDTO vitals = Engine::EntityVitalsDTO::fromJson( json );

        if ( vitals.idCharacter() != _character.idCharacter() ) {
            return;
        }

        Engine::EntityVitalsModel vitalsModel = _character.vitals();
        vitalsModel.setHealth( vitals.health() );
        vitalsModel.setMaxHealth( vitals.maxHealth() );
        vitalsModel.setMana( vitals.mana() );
        vitalsModel.setMaxMana( vitals.maxMana() );
        vitalsModel.setStamina( vitals.stamina() );
        vitalsModel.setMaxStamina( vitals.maxStamina() );
        _character.setVitals( vitalsModel );

        emit vitalsChanged();
        return;
    }

    Engine::EntityStateDTO state = Engine::EntityStateDTO::fromJson( json );
    const QString orientation = QString::fromStdString( Engine::EntityOrientationModel::toString( state.orientation() ) );

    emit entityStateReceived( state.idCharacter(), state.x(), state.y(), state.z(), orientation );

    if ( state.idCharacter() != _character.idCharacter() ) {
        return;
    }

    Engine::EntityPositionModel position = _character.position();
    position.setX( state.x() );
    position.setY( state.y() );
    position.setZ( state.z() );
    _character.setPosition( position );

    Engine::EntityOrientationModel orientationModel = _character.orientation();
    orientationModel.setDirection( state.orientation() );
    _character.setOrientation( orientationModel );

    emit worldEntryReceived();
}
