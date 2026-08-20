#include "DataManager.h"

#include <json/json.h>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/DataFactory.h>

namespace Engine {

DataManager::DataManager() :
    _manifest(),
    _groundCatalog() {
}

DataManager::~DataManager() = default;

void DataManager::initialize( const std::string& configPath ) {
    Engine::DataFactory factory;
    factory.createGroundCatalog( QString::fromStdString( configPath ), _groundCatalog );
}

void DataManager::finalize() {
}

const ManifestModel& DataManager::manifest() const {
    return _manifest;
}

const GroundCatalog& DataManager::groundCatalog() const {
    return _groundCatalog;
}

} // namespace Engine
