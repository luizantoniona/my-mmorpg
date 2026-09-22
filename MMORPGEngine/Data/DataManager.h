#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <string>

#include <MMORPGEngine/Data/Item/ItemCatalog.h>
#include <MMORPGEngine/Data/Item/ItemTypeCatalog.h>
#include <MMORPGEngine/Data/Manifest/ManifestModel.h>
#include <MMORPGEngine/Data/Object/ObjectCatalog.h>
#include <MMORPGEngine/Data/Skill/SkillCatalog.h>
#include <MMORPGEngine/Data/Tile/TileCatalog.h>

namespace Engine {

class DataManager {
public:
    DataManager();
    ~DataManager();

    void initialize( const std::string& configPath );
    void reload( const std::string& configPath );
    void finalize();

    const std::string& configPath() const;

    ManifestModel manifest() const;

    const ObjectCatalog& objectCatalog() const;
    void addObject( const ObjectModel& object );

    const TileCatalog& tileCatalog() const;
    void addTile( const TileModel& tile );

    const ItemTypeCatalog& itemTypeCatalog() const;
    void addItemType( const ItemTypeModel& itemType );

    const ItemCatalog& itemCatalog() const;
    void addItem( const ItemModel& item );

    const SkillCatalog& skillCatalog() const;
    void addSkillTree( const SkillTreeModel& skillTree );

private:
    std::string _configPath;
    ObjectCatalog _objectCatalog;
    TileCatalog _tileCatalog;
    ItemTypeCatalog _itemTypeCatalog;
    ItemCatalog _itemCatalog;
    SkillCatalog _skillCatalog;
};

} // namespace Engine

#endif // DATAMANAGER_H
