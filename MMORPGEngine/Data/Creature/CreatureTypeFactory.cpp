#include "CreatureTypeFactory.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Creature/CreatureTypeModel.h>
#include <MMORPGEngine/Data/DataFactory.h>

namespace Engine {

namespace {

QString indexFilePath( const QString& mapPath ) {
    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    return mapPath + QString( mapJson[ "Catalogs" ][ "Creatures" ].asCString() );
}

} // namespace

bool CreatureTypeFactory::isValidType( const QString& type ) {
    static const QRegularExpression pattern( "^[A-Z0-9_]+$" );

    return pattern.match( type ).hasMatch();
}

QStringList CreatureTypeFactory::readTypeKeys( const QString& configPath ) {
    const QString mapPath = DataFactory::mapPath( configPath );

    const Json::Value json = JsonHelper::loadJsonFile( indexFilePath( mapPath ) );

    QStringList keys;

    for ( const Json::Value& keyJson : json[ "Creatures" ] ) {
        if ( !keyJson.isString() ) {
            qWarning() << "CreatureTypeFactory::readTypeKeys"
                       << "Invalid Creature key, skipping";
            continue;
        }

        const QString key = QString::fromStdString( keyJson.asString() );

        if ( !isValidType( key ) ) {
            qWarning() << "CreatureTypeFactory::readTypeKeys"
                       << "Invalid Creature key (use A-Z, 0-9, _), skipping:" << key;
            continue;
        }

        if ( keys.contains( key ) ) {
            qWarning() << "CreatureTypeFactory::readTypeKeys"
                       << "Duplicated Creature key, skipping:" << key;
            continue;
        }

        keys.append( key );
    }

    return keys;
}

QString CreatureTypeFactory::typeFilePath( const QString& configPath, const QString& type ) {
    return DataFactory::mapPath( configPath ) + "Creatures/" + type + "/" + type + ".json";
}

void CreatureTypeFactory::createCreatureTypeCatalog( const QString& configPath, CreatureTypeCatalog& creatureTypeCatalog ) {
    qInfo() << "CreatureTypeFactory::createCreatureTypeCatalog"
            << "[CREATURES_FOLDER_PATH]" << DataFactory::mapPath( configPath ) + "Creatures/";

    for ( const QString& key : readTypeKeys( configPath ) ) {
        const QString file = typeFilePath( configPath, key );

        if ( !QFile::exists( file ) ) {
            qWarning() << "CreatureTypeFactory::createCreatureTypeCatalog"
                       << "Creature file not found, skipping:" << file;
            continue;
        }

        const Json::Value creatureTypeJson = JsonHelper::loadJsonFile( file );

        if ( !creatureTypeJson.isMember( "Name" ) || !creatureTypeJson.isMember( "Vitals" ) || !creatureTypeJson[ "Vitals" ].isMember( "MaxHealth" ) ) {
            qWarning() << "CreatureTypeFactory::createCreatureTypeCatalog"
                       << "Invalid Creature, skipping" << key;
            continue;
        }

        CreatureTypeModel creatureType;
        creatureType.setType( key );
        creatureType.setName( QString( creatureTypeJson[ "Name" ].asCString() ) );
        creatureType.vitals().setMaxHealth( creatureTypeJson[ "Vitals" ][ "MaxHealth" ].asDouble() );
        creatureType.setAggroRadius( creatureTypeJson.get( "AggroRadius", 0 ).asUInt() );
        creatureType.setAttackRange( creatureTypeJson.get( "AttackRange", 1 ).asUInt() );

        const uint32_t footprint = creatureTypeJson.get( "Footprint", 1 ).asUInt();
        if ( footprint == 0 ) {
            qWarning() << "CreatureTypeFactory::createCreatureTypeCatalog"
                       << "Invalid Footprint (must be >= 1), skipping" << key;
            continue;
        }
        creatureType.setFootprint( footprint );

        creatureTypeCatalog.addCreatureType( std::move( creatureType ) );
    }

    qInfo() << "CreatureTypeFactory::createCreatureTypeCatalog";
}

void CreatureTypeFactory::saveCreatureTypeCatalog( const QString& configPath, const CreatureTypeCatalog& creatureTypeCatalog ) {
    QStringList keys;
    for ( const QString& key : readTypeKeys( configPath ) ) {
        if ( creatureTypeCatalog.creatureType( key ) != nullptr ) {
            keys.append( key );
        }
    }

    QStringList newKeys;
    for ( const auto& entry : creatureTypeCatalog.creatureTypes() ) {
        if ( !keys.contains( entry.first ) ) {
            newKeys.append( entry.first );
        }
    }
    newKeys.sort();
    keys.append( newKeys );

    Json::Value indexJson;
    indexJson[ "Creatures" ] = Json::Value( Json::arrayValue );

    for ( const QString& key : keys ) {
        if ( !isValidType( key ) ) {
            qWarning() << "CreatureTypeFactory::saveCreatureTypeCatalog"
                       << "Invalid Creature key, skipping:" << key;
            continue;
        }

        const CreatureTypeModel& creatureType = *creatureTypeCatalog.creatureType( key );

        const QString file = typeFilePath( configPath, key );
        QDir().mkpath( QFileInfo( file ).absolutePath() );

        Json::Value creatureTypeJson = QFile::exists( file ) ? JsonHelper::loadJsonFile( file ) : Json::Value( Json::objectValue );

        creatureTypeJson[ "Name" ] = creatureType.name().toStdString();
        creatureTypeJson[ "Vitals" ][ "MaxHealth" ] = creatureType.vitals().maxHealth();
        creatureTypeJson[ "AggroRadius" ] = creatureType.aggroRadius();
        creatureTypeJson[ "AttackRange" ] = creatureType.attackRange();
        creatureTypeJson[ "Footprint" ] = creatureType.footprint();

        JsonHelper::saveJsonFile( file, creatureTypeJson );

        indexJson[ "Creatures" ].append( key.toStdString() );
    }

    const QString indexFile = indexFilePath( DataFactory::mapPath( configPath ) );

    qInfo() << "CreatureTypeFactory::saveCreatureTypeCatalog"
            << "[CREATURES_INDEX_FILE_PATH]" << indexFile;

    JsonHelper::saveJsonFile( indexFile, indexJson );
}

} // namespace Engine
