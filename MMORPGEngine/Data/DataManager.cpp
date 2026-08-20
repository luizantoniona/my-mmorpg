#include "DataManager.h"

#include <json/json.h>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/DataFactory.h>

namespace Engine {

DataManager::DataManager() :
    _manifest(),
    _objectTextureCatalog(),
    _tileTextureCatalog() {
}

DataManager::~DataManager() = default;

void DataManager::initialize( const std::string& configPath ) {
    Engine::DataFactory factory;

    factory.createTileTextureCatalog( QString::fromStdString( configPath ), _tileTextureCatalog );
}

void DataManager::finalize() {
}

const ManifestModel& DataManager::manifest() const {
    return _manifest;
}

const TileTextureCatalog& DataManager::tileTextureCatalog() const {
    return _tileTextureCatalog;
}

} // namespace Engine
