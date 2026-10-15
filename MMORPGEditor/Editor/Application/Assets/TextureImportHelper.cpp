#include "TextureImportHelper.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QUrl>

#include <MMORPGEngine/Data/DataFactory.h>

QString TextureImportHelper::toLocalFilePath( const QString& path ) {
    if ( path.startsWith( "file:" ) ) {
        return QUrl( path ).toLocalFile();
    }

    return path;
}

bool TextureImportHelper::isValidFileName( const QString& fileName ) {
    static const QRegularExpression validName( "^[a-z0-9_-]+$" );

    return validName.match( fileName ).hasMatch();
}

bool TextureImportHelper::isAnimatedSource( const QString& sourcePath ) {
    const QString sourceExtension = "." + QFileInfo( sourcePath ).suffix().toLower();

    return Engine::DataFactory::textureExtensions().value( sourceExtension, false );
}

QString TextureImportHelper::destinationExtension( const QString& sourcePath ) {
    if ( !isAnimatedSource( sourcePath ) ) {
        return ".png";
    }

    return "." + QFileInfo( sourcePath ).suffix().toLower();
}

bool TextureImportHelper::importTexture( const QString& sourcePath, const QString& mapPath, const QString& textureFile, const QString& extension ) {
    const QString destinationPath = mapPath + textureFile + extension;

    QDir().mkpath( QFileInfo( destinationPath ).absolutePath() );

    const bool isSameFile = QFileInfo::exists( destinationPath ) && QFileInfo( sourcePath ).canonicalFilePath() == QFileInfo( destinationPath ).canonicalFilePath();

    if ( !isSameFile ) {
        const QString temporaryPath = destinationPath + ".tmp";

        QFile::remove( temporaryPath );

        if ( !QFile::copy( sourcePath, temporaryPath ) ) {
            return false;
        }

        QFile::remove( destinationPath );

        if ( !QFile::rename( temporaryPath, destinationPath ) ) {
            QFile::remove( temporaryPath );
            return false;
        }
    }

    for ( auto it = Engine::DataFactory::textureExtensions().constBegin(); it != Engine::DataFactory::textureExtensions().constEnd(); ++it ) {
        if ( it.key() != extension ) {
            QFile::remove( mapPath + textureFile + it.key() );
        }
    }

    return true;
}

void TextureImportHelper::removeTextureFiles( const QString& mapPath, const QString& textureFile ) {
    for ( auto it = Engine::DataFactory::textureExtensions().constBegin(); it != Engine::DataFactory::textureExtensions().constEnd(); ++it ) {
        QFile::remove( mapPath + textureFile + it.key() );
    }
}
