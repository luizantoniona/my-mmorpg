#include <drogon/drogon.h>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGServer/Server/Database/Database.h>
#include <MMORPGServer/Server/Manager/DataManager.h>
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

    drogon::app()
        .addListener( "0.0.0.0", 8080 )
        .setThreadNum( std::thread::hardware_concurrency() )
        .registerPostHandlingAdvice( []( const drogon::HttpRequestPtr& request, const drogon::HttpResponsePtr& response ) {
            response->addHeader( "Access-Control-Allow-Origin", "*" );
            response->addHeader( "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS" );
            response->addHeader( "Access-Control-Allow-Headers", "Content-Type, Authorization" );
        } )
        .run();

    std::cout << "Ending Server" << std::endl;

    return 0;
}
