#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <MMORPGEngine/Data/Manifest/ManifestModel.h>
#include <MMORPGEngine/Data/Object/ObjectTextureCatalog.h>
#include <MMORPGEngine/Data/Tile/TileTextureCatalog.h>

namespace Engine {

class DataManager {
public:
    DataManager();
    ~DataManager();

    void initialize( const std::string& configPath );
    void finalize();

    const ManifestModel& manifest() const;

    const ObjectTextureCatalog& objectTextureCatalog() const;

    const TileTextureCatalog& tileTextureCatalog() const;

private:
    ManifestModel _manifest;
    ObjectTextureCatalog _objectTextureCatalog;
    TileTextureCatalog _tileTextureCatalog;
};

} // namespace Engine

#endif // DATAMANAGER_H
