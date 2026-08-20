#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <MMORPGEngine/Data/Ground/GroundCatalog.h>
#include <MMORPGEngine/Data/Manifest/ManifestModel.h>

namespace Engine {

class DataManager {
public:
    DataManager();
    ~DataManager();

    void initialize( const std::string& configPath );
    void finalize();

    const ManifestModel& manifest() const;

    const GroundCatalog& groundCatalog() const;

private:
    ManifestModel _manifest;

    GroundCatalog _groundCatalog;
};

} // namespace Engine

#endif // DATAMANAGER_H
