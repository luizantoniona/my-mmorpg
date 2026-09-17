#include "GamePageControl.h"

#include <QUrl>

#include <MMORPGClient/Client/Manager/AccountManager.h>
#include <MMORPGClient/Client/Manager/ServerManager.h>
#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/Character/CharacterDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterMoveDTO.h>
#include <MMORPGEngine/Entity/Character/OwnCharacterDTO.h>
#include <MMORPGEngine/Entity/Creature/CreatureDTO.h>
#include <MMORPGEngine/Entity/EntityLeftDTO.h>
#include <MMORPGEngine/Entity/EntityVitalsModel.h>
#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>
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
    Engine::CharacterMoveDTO input;
    input.setDx( dx );
    input.setDy( dy );

    _webSocket.sendMessage( QString::fromStdString( Engine::JsonHelper::writeJsonString( input.toJson() ) ) );
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

    const Engine::ServerMessageType type = Engine::ServerMessageTypeHelper::fromMessage( json );

    switch ( type ) {
    case Engine::ServerMessageType::ENTITY_LEFT: {
        const Engine::EntityLeftDTO entityLeft = Engine::EntityLeftDTO::fromJson( json );
        emit entityLeftReceived( entityLeft.idCharacter() );
        return;
    }

    case Engine::ServerMessageType::OWN_CHARACTER: {
        Engine::OwnCharacterDTO state = Engine::OwnCharacterDTO::fromJson( json );

        emit entityStateReceived( state.idCharacter(), state.x(), state.y(), state.z() );

        Engine::EntityPositionModel position = _character.position();
        position.setX( state.x() );
        position.setY( state.y() );
        position.setZ( state.z() );
        _character.setPosition( position );

        Engine::EntityVitalsModel vitalsModel = _character.vitals();
        vitalsModel.setHealth( state.health() );
        vitalsModel.setMaxHealth( state.maxHealth() );
        vitalsModel.setMana( state.mana() );
        vitalsModel.setMaxMana( state.maxMana() );
        vitalsModel.setStamina( state.stamina() );
        vitalsModel.setMaxStamina( state.maxStamina() );
        _character.setVitals( vitalsModel );

        emit vitalsChanged();
        emit worldEntryReceived();
        return;
    }

    case Engine::ServerMessageType::CHARACTER: {
        Engine::CharacterDTO state = Engine::CharacterDTO::fromJson( json );

        emit entityStateReceived( state.idCharacter(), state.x(), state.y(), state.z() );
        return;
    }

    case Engine::ServerMessageType::CREATURE: {
        Engine::CreatureDTO state = Engine::CreatureDTO::fromJson( json );

        emit entityStateReceived( state.idCreature(), state.x(), state.y(), state.z() );
        return;
    }

    case Engine::ServerMessageType::WORLD_BASIC:
    case Engine::ServerMessageType::UNKNOWN:
        return;
    }
}
