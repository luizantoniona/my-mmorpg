#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <MMORPGEngine/Data/Manifest/ManifestModel.h>
#include <MMORPGEngine/Data/Object/ObjectCatalog.h>
#include <MMORPGEngine/Data/Tile/TileCatalog.h>

namespace Engine {

class DataManager {
public:
    DataManager();
    ~DataManager();

    void initialize( const std::string& configPath );
    void finalize();

    const ManifestModel& manifest() const;

    const ObjectCatalog& objectCatalog() const;

    const TileCatalog& tileCatalog() const;

private:
    ManifestModel _manifest;
    ObjectCatalog _objectCatalog;
    TileCatalog _tileCatalog;
};

} // namespace Engine

#endif // DATAMANAGER_H
