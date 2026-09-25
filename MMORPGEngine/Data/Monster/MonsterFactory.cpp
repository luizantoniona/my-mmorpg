#include "MonsterFactory.h"

#include <QDebug>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/DataFactory.h>
#include <MMORPGEngine/Data/Monster/MonsterModel.h>

namespace Engine {

void MonsterFactory::createMonsterCatalog( const QString& configPath, MonsterCatalog& monsterCatalog ) {
    const QString mapPath = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    const QString monstersFile = mapPath + QString( mapJson[ "Catalogs" ][ "Monsters" ].asCString() );

    qInfo() << "MonsterFactory::createMonsterCatalog" << "[MONSTERS_FILE_PATH]" << monstersFile;

    Json::Value json = JsonHelper::loadJsonFile( monstersFile );

    const Json::Value& monsters = json[ "Monsters" ];

    for ( const Json::Value& monsterJson : monsters ) {

        if ( !monsterJson.isMember( "Type" ) || !monsterJson.isMember( "Name" ) ) {
            qWarning() << "MonsterFactory::createMonsterCatalog" << "Invalid Monster, skipping";
            continue;
        }

        MonsterModel monster;
        monster.setType( monsterJson[ "Type" ].asUInt() );
        monster.setName( QString( monsterJson[ "Name" ].asCString() ) );

        monsterCatalog.addMonster( std::move( monster ) );
    }

    qInfo() << "MonsterFactory::createMonsterCatalog";
}

void MonsterFactory::saveMonsterCatalog( const QString& configPath, const MonsterCatalog& monsterCatalog ) {
    const QString path = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( path + "Map.json" );

    const QString monstersFile = path + QString( mapJson[ "Catalogs" ][ "Monsters" ].asCString() );

    Json::Value json;
    json[ "Monsters" ] = Json::Value( Json::arrayValue );

    for ( const auto& entry : monsterCatalog.monsters() ) {
        const MonsterModel& monster = entry.second;

        Json::Value monsterJson;
        monsterJson[ "Type" ] = monster.type();
        monsterJson[ "Name" ] = monster.name().toStdString();

        json[ "Monsters" ].append( monsterJson );
    }

    qInfo() << "MonsterFactory::saveMonsterCatalog" << "[MONSTERS_FILE_PATH]" << monstersFile;

    JsonHelper::saveJsonFile( monstersFile, json );
}

} // namespace Engine
