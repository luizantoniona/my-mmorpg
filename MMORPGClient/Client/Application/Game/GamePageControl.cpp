#include "GamePageControl.h"

#include <QUrl>

#include <MMORPGClient/Client/Manager/AccountManager.h>
#include <MMORPGClient/Client/Manager/ServerManager.h>
#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Entity/Character/CharacterIntentActionDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterIntentMoveDTO.h>
#include <MMORPGEngine/Entity/Character/OwnCharacterDTO.h>
#include <MMORPGEngine/Entity/CombatActionHelper.h>
#include <MMORPGEngine/Entity/EntityVitalsModel.h>
#include <MMORPGEngine/Network/WebSocket/ServerMessageReceiver.h>
#include <MMORPGEngine/World/WorldFactory.h>

GamePageControl::GamePageControl( QObject* parent ) :
    QObject( parent ),
    _world( nullptr ),
    _webSocket( this ),
    _character() {

    _character.setIdCharacter( -1 );

    Engine::ServerMessageReceiver& messageReceiver = Engine::Singleton<Engine::ServerMessageReceiver>::instance();

    connect( &_webSocket, &Engine::WebSocketClient::messageReceived, &messageReceiver, &Engine::ServerMessageReceiver::receiveMessage );
    connect( &_webSocket, &Engine::WebSocketClient::errorOccurred, this, &GamePageControl::worldEntryFailed );
    connect( &_webSocket, &Engine::WebSocketClient::disconnected, this, &GamePageControl::worldLeft );

    connect( &messageReceiver, &Engine::ServerMessageReceiver::errorReceived, this, &GamePageControl::worldEntryFailed );
    connect( &messageReceiver, &Engine::ServerMessageReceiver::ownCharacterReceived, this, &GamePageControl::onOwnCharacterReceived );
    connect( &messageReceiver, &Engine::ServerMessageReceiver::ownCombatReceived, this, &GamePageControl::onOwnCombatReceived );
    connect( &messageReceiver, &Engine::ServerMessageReceiver::characterEventAttackStartReceived, this, &GamePageControl::characterEventAttackStartReceived );
    connect( &messageReceiver, &Engine::ServerMessageReceiver::characterEventAttackReceived, this, &GamePageControl::characterEventAttackReceived );
    connect( &messageReceiver, &Engine::ServerMessageReceiver::creatureEventAttackStartReceived, this, &GamePageControl::creatureEventAttackStartReceived );
    connect( &messageReceiver, &Engine::ServerMessageReceiver::creatureEventAttackReceived, this, &GamePageControl::creatureEventAttackReceived );
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

int GamePageControl::characterX() const {
    return _character.position().x();
}

int GamePageControl::characterY() const {
    return _character.position().y();
}

int GamePageControl::characterZ() const {
    return _character.position().z();
}

QString GamePageControl::secondAction() const {
    const Engine::EntityCombatModel& combat = _character.combat();

    if ( combat.isActionAvailable( Engine::CombatActionEnum::BLOCK ) ) {
        return QString::fromStdString( Engine::CombatActionHelper::toString( Engine::CombatActionEnum::BLOCK ) );
    }

    if ( combat.isActionAvailable( Engine::CombatActionEnum::DODGE ) ) {
        return QString::fromStdString( Engine::CombatActionHelper::toString( Engine::CombatActionEnum::DODGE ) );
    }

    return "";
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
    Engine::CharacterIntentMoveDTO input;
    input.setDx( dx );
    input.setDy( dy );

    _webSocket.sendMessage( QString::fromStdString( Engine::JsonHelper::writeJsonString( input.toJson() ) ) );
}

void GamePageControl::attack( int dx, int dy ) {
    sendAction( Engine::CombatActionEnum::ATTACK, dx, dy );
}

void GamePageControl::useSecondAction() {
    const Engine::EntityCombatModel& combat = _character.combat();

    if ( combat.isActionAvailable( Engine::CombatActionEnum::BLOCK ) ) {
        sendAction( Engine::CombatActionEnum::BLOCK, 0, 0 );
    } else if ( combat.isActionAvailable( Engine::CombatActionEnum::DODGE ) ) {
        sendAction( Engine::CombatActionEnum::DODGE, 0, 0 );
    }
}

void GamePageControl::leaveWorld() {
    _webSocket.disconnectFromServer();
}

void GamePageControl::onOwnCharacterReceived( const Engine::OwnCharacterDTO& state ) {
    _character.position().setX( state.x() );
    _character.position().setY( state.y() );
    _character.position().setZ( state.z() );

    _character.vitals().setHealth( state.health() );
    _character.vitals().setMaxHealth( state.maxHealth() );
    _character.vitals().setMana( state.mana() );
    _character.vitals().setMaxMana( state.maxMana() );
    _character.vitals().setStamina( state.stamina() );
    _character.vitals().setMaxStamina( state.maxStamina() );

    emit positionChanged();
    emit vitalsChanged();
    emit worldEntryReceived();
}

void GamePageControl::onOwnCombatReceived( const Engine::OwnCombatDTO& state ) {
    _character.combat().setAvailableActions( state.actions() );

    emit combatChanged();
}

void GamePageControl::sendAction( Engine::CombatActionEnum action, int dx, int dy ) {
    Engine::CharacterIntentActionDTO input;
    input.setAction( action );
    input.setDx( dx );
    input.setDy( dy );

    _webSocket.sendMessage( QString::fromStdString( Engine::JsonHelper::writeJsonString( input.toJson() ) ) );
}
