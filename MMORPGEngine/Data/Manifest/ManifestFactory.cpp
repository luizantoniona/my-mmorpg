#include "ManifestFactory.h"

#include <QCryptographicHash>
#include <QDir>
#include <QDirIterator>
#include <QFile>

#include <MMORPGEngine/Commons/JsonHelper.h>

namespace Engine {

ManifestModel ManifestFactory::createManifest( const std::string& configPath ) {
    ManifestModel manifest;

    const QString configPathQ = QDir( QString::fromStdString( configPath ) ).absolutePath() + "/";
    const QString configFilePath = configPathQ + "Config.json";

    manifest.addData( ManifestDataModel( "Config.json", hashFile( configFilePath ) ) );

    Json::Value configJson = JsonHelper::loadJsonFile( configFilePath );
    const QString mapFolder = QString::fromStdString( configJson[ "ActiveFolder" ].asString() );
    const QString mapPath = configPathQ + mapFolder + "/";
    const QDir mapDir( mapPath );

    QDirIterator it( mapPath, QDir::Files, QDirIterator::Subdirectories );
    while ( it.hasNext() ) {
        const QString filePath = it.next();
        const QString relativePath = mapFolder + "/" + mapDir.relativeFilePath( filePath );

        manifest.addData( ManifestDataModel( relativePath.toStdString(), hashFile( filePath ) ) );
    }

    return manifest;
}

std::string ManifestFactory::hashFile( const QString& path ) {
    QFile file( path );

    if ( !file.open( QIODevice::ReadOnly ) ) {
        return "";
    }

    QCryptographicHash hash( QCryptographicHash::Md5 );
    hash.addData( &file );

    return QString::fromLatin1( hash.result().toHex() ).toStdString();
}

} // namespace Engine
