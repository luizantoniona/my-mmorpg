#include "DataManager.h"

#include <json/json.h>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Object/ObjectFactory.h>
#include <MMORPGEngine/Data/Tile/TileFactory.h>

namespace Engine {

DataManager::DataManager() :
    _manifest(),
    _objectCatalog(),
    _tileCatalog() {
}

DataManager::~DataManager() = default;

void DataManager::initialize( const std::string& configPath ) {
    ObjectFactory::createObjectCatalog( QString::fromStdString( configPath ), _objectCatalog );
    TileFactory::createTileCatalog( QString::fromStdString( configPath ), _tileCatalog );
}

void DataManager::reload( const std::string& configPath ) {
    _objectCatalog = ObjectCatalog();
    _tileCatalog = TileCatalog();

    initialize( configPath );
}

void DataManager::finalize() {
}

const ManifestModel& DataManager::manifest() const {
    return _manifest;
}

const ObjectCatalog& DataManager::objectCatalog() const {
    return _objectCatalog;
}

void DataManager::addObject( const ObjectModel& object ) {
    _objectCatalog.addObject( object );
}

const TileCatalog& DataManager::tileCatalog() const {
    return _tileCatalog;
}

void DataManager::addTile( const TileModel& tile ) {
    _tileCatalog.addTile( tile );
}

} // namespace Engine
