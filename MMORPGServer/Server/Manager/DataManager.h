#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <MMORPGEngine/Data/Ground/GroundCatalog.h>
#include <MMORPGEngine/Data/Manifest/ManifestModel.h>

namespace Server {

class DataManager {
public:
    DataManager();
    ~DataManager();

    void initialize( const std::string& worldPath );
    void finalize();

    const Engine::ManifestModel& manifest() const;
    const Engine::GroundCatalog& groundCatalog() const;

private:
    Engine::ManifestModel _manifest;
    Engine::GroundCatalog _groundCatalog;
};

} // namespace Server

#endif // DATAMANAGER_H
