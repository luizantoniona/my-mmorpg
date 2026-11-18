#include "CharacterCreationPageControl.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

#include <MMORPGClient/Client/Manager/AccountManager.h>
#include <MMORPGClient/Client/Manager/ServerManager.h>
#include <MMORPGEngine/Account/AccountCharacterDTO.h>
#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Entity/Character/CharacterAppearanceFeatureHelper.h>

CharacterCreationPageControl::CharacterCreationPageControl( QObject* parent ) :
    QObject( parent ),
    _appearance() {
}

CharacterCreationPageControl::~CharacterCreationPageControl() = default;

QVariantMap CharacterCreationPageControl::appearance() const {
    QVariantMap selections;

    for ( const auto& selection : _appearance.selections() ) {
        selections[ QString::fromStdString( Engine::CharacterAppearanceFeatureHelper::toString( selection.first ) ) ] = selection.second;
    }

    return selections;
}

bool CharacterCreationPageControl::hasValidAppearance() const {
    return Engine::Singleton<Engine::DataManager>::instance().characterCatalog().isValidAppearance( _appearance );
}

QStringList CharacterCreationPageControl::appearanceFeatures() const {
    QStringList features;

    for ( const Engine::CharacterAppearanceFeatureEnum feature : Engine::Singleton<Engine::DataManager>::instance().characterCatalog().features() ) {
        features.append( QString::fromStdString( Engine::CharacterAppearanceFeatureHelper::toString( feature ) ) );
    }

    return features;
}

QString CharacterCreationPageControl::appearanceFeatureTitle( const QString& feature ) const {
    switch ( Engine::CharacterAppearanceFeatureHelper::fromString( feature.toStdString() ) ) {
    case Engine::CharacterAppearanceFeatureEnum::BODY:
        return tr( "Body" );
    case Engine::CharacterAppearanceFeatureEnum::UNKNOWN:
        return QString();
    }

    return QString();
}

QVariantList CharacterCreationPageControl::appearanceOptions( const QString& feature, const QVariantMap& selection ) const {
    QVariantList result;

    const Engine::CharacterCatalog& characterCatalog = Engine::Singleton<Engine::DataManager>::instance().characterCatalog();

    for ( const Engine::CharacterOptionModel& option : characterCatalog.options( Engine::CharacterAppearanceFeatureHelper::fromString( feature.toStdString() ), appearanceFromSelections( selection ) ) ) {
        QVariantMap entry;
        entry[ "id" ] = option.id();
        entry[ "name" ] = option.name();

        result.append( entry );
    }

    return result;
}

Engine::CharacterAppearanceModel CharacterCreationPageControl::appearanceFromSelections( const QVariantMap& selections ) {
    Engine::CharacterAppearanceModel appearance;

    for ( auto selection = selections.constBegin(); selection != selections.constEnd(); ++selection ) {
        const Engine::CharacterAppearanceFeatureEnum feature = Engine::CharacterAppearanceFeatureHelper::fromString( selection.key().toStdString() );

        if ( feature != Engine::CharacterAppearanceFeatureEnum::UNKNOWN ) {
            appearance.setId( feature, selection.value().toString() );
        }
    }

    return appearance;
}

void CharacterCreationPageControl::selectAppearanceOption( const QString& feature, const QString& id ) {
    const Engine::CharacterAppearanceFeatureEnum appearanceFeature = Engine::CharacterAppearanceFeatureHelper::fromString( feature.toStdString() );

    if ( appearanceFeature == Engine::CharacterAppearanceFeatureEnum::UNKNOWN || _appearance.id( appearanceFeature ) == id ) {
        return;
    }

    _appearance.setId( appearanceFeature, id );

    emit appearanceChanged();
}

void CharacterCreationPageControl::createCharacter( const QString& name ) {
    emit characterCreationFailed( "" );

    if ( name.trimmed().isEmpty() ) {
        emit characterCreationFailed( tr( "Name is required" ) );
        return;
    }

    if ( !hasValidAppearance() ) {
        emit characterCreationFailed( tr( "Choose a valid appearance" ) );
        return;
    }

    ServerManager& serverManager = Engine::Singleton<ServerManager>::instance();
    if ( serverManager.connectionState() != ServerManager::ConnectionState::Connected ) {
        emit characterCreationFailed( tr( "Not connected to server" ) );
        return;
    }

    QJsonObject appearanceJson;
    for ( const auto& entry : _appearance.selections() ) {
        appearanceJson[ QString::fromStdString( Engine::CharacterAppearanceFeatureHelper::toString( entry.first ) ) ] = entry.second;
    }

    QJsonObject requestJson;
    requestJson[ "name" ] = name.trimmed();
    requestJson[ "appearance" ] = appearanceJson;

    QNetworkReply* reply = serverManager.postAuthenticated( "/create", QJsonDocument( requestJson ).toJson( QJsonDocument::Compact ) );
    connect( reply, &QNetworkReply::finished, this, [ this, reply, name ]() {
        reply->deleteLater();

        if ( reply->error() != QNetworkReply::NoError ) {
            const QJsonObject errorJson = QJsonDocument::fromJson( reply->readAll() ).object();

            if ( errorJson[ "error" ].toString() == "invalid_appearance" ) {
                emit characterCreationFailed( tr( "The server rejected the chosen appearance" ) );
                return;
            }

            emit characterCreationFailed( reply->errorString() );
            return;
        }

        const QByteArray responseData = reply->readAll();

        Json::Value responseJson = Engine::JsonHelper::parseJsonString( responseData.toStdString() );

        if ( responseJson.isNull() || !responseJson.isMember( "idCharacter" ) ) {
            emit characterCreationFailed( tr( "Invalid server response" ) );
            return;
        }

        Engine::AccountCharacterDTO character;
        character.setIdCharacter( responseJson[ "idCharacter" ].asInt() );
        character.setName( name.trimmed().toStdString() );

        Engine::Singleton<AccountManager>::instance().addCharacter( character );

        emit characterCreated();
    } );
}
