#include "CreatureTypeFactory.h"

#include <QDebug>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Creature/CreatureTypeModel.h>
#include <MMORPGEngine/Data/DataFactory.h>

namespace Engine {

void CreatureTypeFactory::createCreatureTypeCatalog( const QString& configPath, CreatureTypeCatalog& creatureTypeCatalog ) {
    const QString mapPath = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    const QString creaturesFile = mapPath + QString( mapJson[ "Catalogs" ][ "Creatures" ].asCString() );

    qInfo() << "CreatureTypeFactory::createCreatureTypeCatalog [CREATURES_FILE_PATH]" << creaturesFile;

    Json::Value json = JsonHelper::loadJsonFile( creaturesFile );

    const Json::Value& creatureTypes = json[ "Creatures" ];

    for ( const Json::Value& creatureTypeJson : creatureTypes ) {

        if ( !creatureTypeJson.isMember( "Type" ) || !creatureTypeJson.isMember( "Name" ) || !creatureTypeJson.isMember( "Vitals" ) || !creatureTypeJson[ "Vitals" ].isMember( "MaxHealth" ) ) {
            qWarning() << "CreatureTypeFactory::createCreatureTypeCatalog Invalid Creature, skipping";
            continue;
        }

        CreatureTypeModel creatureType;
        creatureType.setType( creatureTypeJson[ "Type" ].asUInt() );
        creatureType.setName( QString( creatureTypeJson[ "Name" ].asCString() ) );
        creatureType.vitals().setMaxHealth( creatureTypeJson[ "Vitals" ][ "MaxHealth" ].asDouble() );

        creatureTypeCatalog.addCreatureType( std::move( creatureType ) );
    }

    qInfo() << "CreatureTypeFactory::createCreatureTypeCatalog";
}

void CreatureTypeFactory::saveCreatureTypeCatalog( const QString& configPath, const CreatureTypeCatalog& creatureTypeCatalog ) {
    const QString path = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( path + "Map.json" );

    const QString creaturesFile = path + QString( mapJson[ "Catalogs" ][ "Creatures" ].asCString() );

    Json::Value json;
    json[ "Creatures" ] = Json::Value( Json::arrayValue );

    for ( const auto& entry : creatureTypeCatalog.creatureTypes() ) {
        const CreatureTypeModel& creatureType = entry.second;

        Json::Value creatureTypeJson;
        creatureTypeJson[ "Type" ] = creatureType.type();
        creatureTypeJson[ "Name" ] = creatureType.name().toStdString();
        creatureTypeJson[ "Vitals" ][ "MaxHealth" ] = creatureType.vitals().maxHealth();

        json[ "Creatures" ].append( creatureTypeJson );
    }

    qInfo() << "CreatureTypeFactory::saveCreatureTypeCatalog [CREATURES_FILE_PATH]" << creaturesFile;

    JsonHelper::saveJsonFile( creaturesFile, json );
}

} // namespace Engine
