#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QSurfaceFormat>

#include <MMORPGEngine/Commons/RegisterEngineTypes.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGServer/Server/Database/Database.h>
#include <MMORPGServer/Server/Manager/DataManager.h>
#include <MMORPGServer/Server/Manager/NetworkManager.h>
#include <MMORPGServer/Server/Manager/WorldManager.h>
#include <MMORPGServer/Server/RegisterServerTypes.h>

namespace {
constexpr const char* DATABASE_PATH = "../../../Database/ServerDatabase";
constexpr const char* DATA_PATH = "../../../Data/";
} // namespace

int main( int argc, char* argv[] ) {
    QQuickStyle::setStyle( "Basic" );
    QGuiApplication app( argc, argv );
    QSurfaceFormat format;
    format.setSamples( 8 );
    QSurfaceFormat::setDefaultFormat( format );
    QQmlApplicationEngine engine;

    // --- Register Types Engine
    Engine::RegisterEngineTypes::registerTypes();

    // --- Register Types Server
    Server::RegisterServerTypes::registerTypes();

    qInfo() << "STARTING SERVER";

    // --- Database ---
    Engine::Singleton<Server::Database>::instance().initialize( DATABASE_PATH );

    // --- Data ---
    Engine::Singleton<Server::DataManager>::instance().initialize();

    // --- World ---
    Engine::Singleton<Server::WorldManager>::instance().initialize( DATA_PATH );

    // --- Network ---
    Engine::Singleton<Server::NetworkManager>::instance().initialize();

    QObject::connect( &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit( -1 ); }, Qt::QueuedConnection );
    engine.loadFromModule( "MMORPGServerComponents", "Main" );

    app.exec();

    // --- Network ---
    Engine::Singleton<Server::NetworkManager>::instance().finalize();

    // --- World ---
    Engine::Singleton<Server::WorldManager>::instance().finalize();

    // --- Data ---
    Engine::Singleton<Server::DataManager>::instance().finalize();

    // --- Database ---
    Engine::Singleton<Server::Database>::instance().finalize();

    qInfo() << "ENDING SERVER";

    return 0;
}
