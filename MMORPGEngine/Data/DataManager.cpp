#include "DataManager.h"

#include <json/json.h>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/DataFactory.h>

namespace Engine {

DataManager::DataManager() :
    _manifest(),
    _objectCatalog(),
    _tileCatalog() {
}

DataManager::~DataManager() = default;

void DataManager::initialize( const std::string& configPath ) {
    Engine::DataFactory factory;
    factory.createObjectCatalog( QString::fromStdString( configPath ), _objectCatalog );
    factory.createTileCatalog( QString::fromStdString( configPath ), _tileCatalog );
}

void DataManager::finalize() {
}

const ManifestModel& DataManager::manifest() const {
    return _manifest;
}

const ObjectCatalog& DataManager::objectCatalog() const {
    return _objectCatalog;
}

const TileCatalog& DataManager::tileCatalog() const {
    return _tileCatalog;
}

} // namespace Engine
