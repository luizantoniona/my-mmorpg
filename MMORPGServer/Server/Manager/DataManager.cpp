#include "DataManager.h"

#include <json/json.h>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/DataFactory.h>

namespace Server {

DataManager::DataManager() :
    _manifest(),
    _groundCatalog() {
}

DataManager::~DataManager() = default;

void DataManager::initialize( const std::string& worldPath ) {
    Json::Value configJson = Engine::JsonHelper::loadJsonFile( worldPath + "Config.json" );

    const QString mapFolder = QString( configJson[ "ActiveFolder" ].asCString() );

    const QString mapPath = QString::fromStdString( worldPath ) + mapFolder + "/";

    Json::Value mapJson = Engine::JsonHelper::loadJsonFile( mapPath + "Map.json" );

    Engine::DataFactory factory;

    const QString groundFile = mapPath + QString( mapJson[ "Catalogs" ][ "Grounds" ].asCString() );
    factory.createGroundCatalog( groundFile, _groundCatalog );
}

void DataManager::finalize() {
}

const Engine::ManifestModel& DataManager::manifest() const {
    return _manifest;
}

const Engine::GroundCatalog& DataManager::groundCatalog() const {
    return _groundCatalog;
}

} // namespace Server
