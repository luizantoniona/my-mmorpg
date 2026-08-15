#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QSurfaceFormat>

#include <MMORPGEditor/Editor/RegisterEditorTypes.h>
#include <MMORPGEngine/Commons/RegisterEngineTypes.h>

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

    QObject::connect( &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit( -1 ); }, Qt::QueuedConnection );
    engine.loadFromModule( "MMORPGEditorComponents", "Main" );

    return app.exec();
}
