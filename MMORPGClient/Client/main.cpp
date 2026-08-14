#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QSurfaceFormat>

#include <MMORPGClient/Client/Manager/AccountManager.h>
#include <MMORPGClient/Client/Manager/ServerManager.h>
#include <MMORPGClient/Client/RegisterTypes.h>
#include <MMORPGEngine/Commons/RegisterEngineTypes.h>
#include <MMORPGEngine/Commons/Singleton.h>

int main( int argc, char* argv[] ) {
    QQuickStyle::setStyle( "Basic" );
    QGuiApplication app( argc, argv );
    QSurfaceFormat format;
    format.setSamples( 8 );
    QSurfaceFormat::setDefaultFormat( format );
    QQmlApplicationEngine engine;

    // --- Register Types Engine
    Engine::RegisterEngineTypes::registerTypes();

    // --- Register Types Client
    Client::RegisterTypes::registerTypes();

    // --- Server configuration
    Engine::Singleton<ServerManager>::instance();

    // --- Account configuration
    Engine::Singleton<AccountManager>::instance();

    QObject::connect( &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit( -1 ); }, Qt::QueuedConnection );
    engine.loadFromModule( "MMORPGClientComponents", "Main" );

    return app.exec();
}
