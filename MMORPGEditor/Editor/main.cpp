#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QSurfaceFormat>

#include <MMORPGEditor/Editor/RegisterEditorTypes.h>
#include <MMORPGEngine/Commons/RegisterEngineTypes.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>

namespace {
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

    // --- Register Types Editor
    Editor::RegisterEditorTypes::registerTypes();

    qInfo() << "STARTING EDITOR";

    // --- Data ---
    Engine::Singleton<Engine::DataManager>::instance().initialize( DATA_PATH );

    QObject::connect( &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit( -1 ); }, Qt::QueuedConnection );
    engine.loadFromModule( "MMORPGEditorComponents", "Main" );

    return app.exec();
}
