#include <drogon/drogon.h>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGServer/Server/Database/Database.h>
#include <MMORPGServer/Server/Manager/DataManager.h>
#include <MMORPGServer/Server/Manager/NetworkManager.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>

namespace {
constexpr const char* DATABASE_PATH = "../../../../Database/ServerDatabase";
constexpr const char* DATA_PATH = "../../../../Data/";
} // namespace

int main() {
    std::cout << "Starting Server" << std::endl;

    // --- Database ---
    Engine::Singleton<Server::Database>::instance().initialize( DATABASE_PATH );

    // --- Data ---
    Engine::Singleton<Server::DataManager>::instance().initialize();

    // --- World ---
    Engine::Singleton<Server::WorldManager>::instance().initialize( DATA_PATH );

    // --- Network ---
    Engine::Singleton<Server::NetworkManager>::instance().initialize();

    while ( true ) {
    }

    std::cout << "Ending Server" << std::endl;

    return 0;
}
