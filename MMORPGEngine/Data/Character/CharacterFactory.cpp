#include "CharacterFactory.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QImageReader>
#include <QRegularExpression>
#include <QSize>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Appearance/AppearanceDirectionHelper.h>
#include <MMORPGEngine/Data/Appearance/AppearanceStateHelper.h>
#include <MMORPGEngine/Data/Character/CharacterLayerHelper.h>
#include <MMORPGEngine/Data/DataFactory.h>

namespace Engine {

namespace {

constexpr int DIRECTION_COUNT = 4;

QString catalogFilePath( const QString& mapPath ) {
    const Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );
    const Json::Value& catalogsJson = mapJson[ "Catalogs" ];

    if ( !catalogsJson.isObject() || !catalogsJson[ "Characters" ].isString() ) {
        return QString();
    }

    return mapPath + QString::fromStdString( catalogsJson[ "Characters" ].asString() );
}

bool readSize( const Json::Value& json, QSize& size ) {
    if ( !json.isArray() || json.size() != 2 || !json[ 0 ].isInt() || !json[ 1 ].isInt() ) {
        return false;
    }

    size = QSize( json[ 0 ].asInt(), json[ 1 ].asInt() );

    return size.width() > 0 && size.height() > 0;
}

bool readRows( const Json::Value& rowsJson, CharacterCatalog& characterCatalog ) {
    if ( !rowsJson.isObject() ) {
        return false;
    }

    for ( const std::string& directionName : rowsJson.getMemberNames() ) {
        const AppearanceDirectionEnum direction = AppearanceDirectionHelper::fromString( directionName );
        const Json::Value& rowJson = rowsJson[ directionName ];

        if ( direction == AppearanceDirectionEnum::UNKNOWN || !rowJson.isInt() || rowJson.asInt() < 0 ) {
            qWarning() << "CharacterFactory::createCharacterCatalog"
                       << "Invalid direction row, skipping" << QString::fromStdString( directionName );
            continue;
        }

        characterCatalog.setRow( direction, rowJson.asInt() );
    }

    return static_cast<int>( characterCatalog.rows().size() ) == DIRECTION_COUNT;
}

bool isSafeRelativePath( const QString& path ) {
    return !path.isEmpty() && !path.contains( ".." ) && !path.contains( ':' ) && !path.startsWith( '\\' ) && !QDir::isAbsolutePath( path );
}

bool hasValidSheetSize( const QString& path, const QSize& frameSize, int frameCount, int rowCount ) {
    const QSize size = QImageReader( path ).size();

    return size.isValid() && size.width() == frameSize.width() * frameCount && size.height() >= frameSize.height() * rowCount;
}

void readClips( const QString& id, const Json::Value& animationsJson, CharacterBodyModel& body ) {
    for ( const std::string& stateName : animationsJson.getMemberNames() ) {
        const AppearanceStateEnum state = AppearanceStateHelper::fromString( stateName );
        const Json::Value& clipJson = animationsJson[ stateName ];

        if ( state == AppearanceStateEnum::UNKNOWN ) {
            qWarning() << "CharacterFactory::createCharacterCatalog"
                       << "Unknown animation state, skipping" << id << QString::fromStdString( stateName );
            continue;
        }

        const bool hasValidClip = JsonHelper::hasValidFields( clipJson, { { "Count", JsonFieldTypeEnum::UINT }, { "FrameDurationMs", JsonFieldTypeEnum::INT } } ) && clipJson.isMember( "Count" ) && clipJson.isMember( "FrameDurationMs" ) && clipJson[ "Count" ].asUInt() > 0 && clipJson[ "FrameDurationMs" ].asInt() > 0;

        if ( !hasValidClip ) {
            qWarning() << "CharacterFactory::createCharacterCatalog"
                       << "Animation needs positive Count and FrameDurationMs, skipping" << id << QString::fromStdString( stateName );
            continue;
        }

        body.setClip( state, CharacterClipModel( static_cast<int>( clipJson[ "Count" ].asUInt() ), clipJson[ "FrameDurationMs" ].asInt() ) );
    }
}

void readLayers( const QString& id, const Json::Value& layersJson, const CharacterCatalog& characterCatalog, CharacterBodyModel& body ) {
    for ( const std::string& layerName : layersJson.getMemberNames() ) {
        const CharacterLayerEnum layer = CharacterLayerHelper::fromString( layerName );
        const Json::Value& filesJson = layersJson[ layerName ];

        if ( layer == CharacterLayerEnum::UNKNOWN || !filesJson.isObject() ) {
            qWarning() << "CharacterFactory::createCharacterCatalog"
                       << "Unknown layer, skipping" << id << QString::fromStdString( layerName );
            continue;
        }

        for ( const std::string& stateName : filesJson.getMemberNames() ) {
            const CharacterClipModel* clip = body.clip( AppearanceStateHelper::fromString( stateName ) );
            const Json::Value& fileJson = filesJson[ stateName ];

            if ( clip == nullptr || !fileJson.isString() || !isSafeRelativePath( QString::fromStdString( fileJson.asString() ) ) ) {
                qWarning() << "CharacterFactory::createCharacterCatalog"
                           << "Layer file without a valid animation, skipping" << id << QString::fromStdString( layerName + "/" + stateName );
                continue;
            }

            const QString file = QString::fromStdString( fileJson.asString() );

            if ( !hasValidSheetSize( body.sprite().folder() + file, characterCatalog.frameSize(), clip->frameCount(), characterCatalog.rowCount() ) ) {
                qWarning() << "CharacterFactory::createCharacterCatalog"
                           << "Sheet missing or with the wrong size, skipping" << body.sprite().folder() + file;
                continue;
            }

            body.sprite().setFile( layer, AppearanceStateHelper::fromString( stateName ), file );
        }
    }
}

} // namespace

bool CharacterFactory::isValidId( const QString& id ) {
    static const QRegularExpression pattern( "^[A-Z0-9_]+$" );

    return pattern.match( id ).hasMatch();
}

void CharacterFactory::createCharacterCatalog( const QString& configPath, CharacterCatalog& characterCatalog ) {
    const QString mapPath = DataFactory::mapPath( configPath );
    const QString file = catalogFilePath( mapPath );

    if ( file.isEmpty() || !QFile::exists( file ) ) {
        qWarning() << "CharacterFactory::createCharacterCatalog"
                   << "No Characters catalog, characters are drawn as circles" << file;
        return;
    }

    qInfo() << "CharacterFactory::createCharacterCatalog"
            << "[CHARACTERS_FILE_PATH]" << file;

    const Json::Value json = JsonHelper::loadJsonFile( file );

    QSize frameSize;
    if ( !json.isObject() || !readSize( json[ "FrameSize" ], frameSize ) ) {
        qWarning() << "CharacterFactory::createCharacterCatalog"
                   << "Invalid FrameSize, skipping the catalog";
        return;
    }

    characterCatalog.setFrameSize( frameSize );

    if ( !readRows( json[ "Rows" ], characterCatalog ) ) {
        qWarning() << "CharacterFactory::createCharacterCatalog"
                   << "Rows needs north, west, south and east, skipping the catalog";
        return;
    }

    const Json::Value& bodiesJson = json[ "Bodies" ];

    if ( !bodiesJson.isObject() ) {
        qWarning() << "CharacterFactory::createCharacterCatalog"
                   << "Catalog without Bodies";
        return;
    }

    for ( const std::string& idName : bodiesJson.getMemberNames() ) {
        const QString id = QString::fromStdString( idName );
        const Json::Value& bodyJson = bodiesJson[ idName ];

        if ( !isValidId( id ) ) {
            qWarning() << "CharacterFactory::createCharacterCatalog"
                       << "Invalid Body id (use A-Z, 0-9, _), skipping" << id;
            continue;
        }

        const bool hasValidBody = JsonHelper::hasValidFields( bodyJson, { { "Name", JsonFieldTypeEnum::STRING }, { "Folder", JsonFieldTypeEnum::STRING }, { "Animations", JsonFieldTypeEnum::OBJECT }, { "Layers", JsonFieldTypeEnum::OBJECT } } ) && bodyJson.isMember( "Name" ) && bodyJson.isMember( "Folder" ) && bodyJson.isMember( "Animations" ) && bodyJson.isMember( "Layers" ) && isSafeRelativePath( QString::fromStdString( bodyJson[ "Folder" ].asString() ) );

        if ( !hasValidBody ) {
            qWarning() << "CharacterFactory::createCharacterCatalog"
                       << "Invalid Body, skipping" << id;
            continue;
        }

        CharacterBodyModel body;
        body.setId( id );
        body.setName( QString::fromStdString( bodyJson[ "Name" ].asString() ) );
        body.sprite().setFolder( mapPath + "Characters/" + QString::fromStdString( bodyJson[ "Folder" ].asString() ) + "/" );

        readClips( id, bodyJson[ "Animations" ], body );
        readLayers( id, bodyJson[ "Layers" ], characterCatalog, body );

        if ( body.sprite().file( CharacterLayerEnum::BODY, AppearanceStateEnum::IDLE ).isEmpty() || body.sprite().file( CharacterLayerEnum::HEAD, AppearanceStateEnum::IDLE ).isEmpty() ) {
            qWarning() << "CharacterFactory::createCharacterCatalog"
                       << "Body needs valid BODY and HEAD layers for idle, skipping" << id;
            continue;
        }

        characterCatalog.addBody( body );
    }

    qInfo() << "CharacterFactory::createCharacterCatalog";
}

} // namespace Engine
