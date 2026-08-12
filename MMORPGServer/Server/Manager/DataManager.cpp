#include "DataManager.h"

namespace Server {

DataManager::DataManager() {
}

DataManager::~DataManager() = default;

void DataManager::initialize() {
}

void DataManager::finalize() {
}

const Engine::ManifestModel& DataManager::manifest() {
    return _manifest;
}

} // namespace Server
