#include "DataManager.h"

#include <json/json.h>

#include <QDebug>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Item/ItemTypeFactory.h>
#include <MMORPGEngine/Data/Manifest/ManifestFactory.h>
#include <MMORPGEngine/Data/Object/ObjectFactory.h>
#include <MMORPGEngine/Data/Skill/SkillFactory.h>
#include <MMORPGEngine/Data/Tile/TileFactory.h>

namespace Engine {

DataManager::DataManager() :
    _configPath(),
    _objectCatalog(),
    _tileCatalog(),
    _itemTypeCatalog(),
    _skillCatalog() {
}

DataManager::~DataManager() = default;

void DataManager::initialize( const std::string& configPath ) {
    _configPath = configPath;

    ObjectFactory::createObjectCatalog( QString::fromStdString( configPath ), _objectCatalog );
    TileFactory::createTileCatalog( QString::fromStdString( configPath ), _tileCatalog );
    ItemTypeFactory::createItemTypeCatalog( QString::fromStdString( configPath ), _itemTypeCatalog );
    SkillFactory::createSkillCatalog( QString::fromStdString( configPath ), _itemTypeCatalog, _skillCatalog );
}

void DataManager::reload( const std::string& configPath ) {
    _objectCatalog = ObjectCatalog();
    _tileCatalog = TileCatalog();
    _itemTypeCatalog = ItemTypeCatalog();
    _skillCatalog = SkillCatalog();

    initialize( configPath );
}

void DataManager::finalize() {
}

const std::string& DataManager::configPath() const {
    return _configPath;
}

ManifestModel DataManager::manifest() const {
    return ManifestFactory::createManifest( _configPath );
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

const ItemTypeCatalog& DataManager::itemTypeCatalog() const {
    return _itemTypeCatalog;
}

void DataManager::addItemType( const ItemTypeModel& itemType ) {
    _itemTypeCatalog.addItemType( itemType );
}

const SkillCatalog& DataManager::skillCatalog() const {
    return _skillCatalog;
}

void DataManager::addSkillTree( const SkillTreeModel& skillTree ) {
    _skillCatalog.addTree( skillTree );
}

} // namespace Engine
