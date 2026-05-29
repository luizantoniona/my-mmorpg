#include <QGuiApplication>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QSurfaceFormat>

int main( int argc, char* argv[] ) {
    QQuickStyle::setStyle( "Basic" );

    QGuiApplication app( argc, argv );

    QSurfaceFormat format;
    format.setSamples( 8 );
    QSurfaceFormat::setDefaultFormat( format );

    QQmlApplicationEngine engine;
    QObject::connect( &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit( -1 ); }, Qt::QueuedConnection );
    engine.loadFromModule( "main", "Main" );

    return app.exec();
}
