#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <MMORPGEngine/Manifest/ManifestModel.h>

namespace Server {

class DataManager {
public:
    DataManager();
    ~DataManager();

    void initialize();
    void finalize();

    const Engine::ManifestModel& manifest();

private:
    Engine::ManifestModel _manifest;
};

} // namespace Server

#endif // DATAMANAGER_H
