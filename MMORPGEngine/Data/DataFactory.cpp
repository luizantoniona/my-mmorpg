#include "DataFactory.h"

#include <QFile>
#include <QImageReader>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>

namespace Engine {

QString DataFactory::mapPath( const QString& configPath ) {
    Json::Value configJson = JsonHelper::loadJsonFile( configPath + "Config.json" );

    const QString mapFolder = QString( configJson[ "ActiveFolder" ].asCString() );

    return configPath + mapFolder + "/";
}

const QMap<QString, bool>& DataFactory::textureExtensions() {
    static const QMap<QString, bool> extensions{
        { ".gif", true },
        { ".png", false },
        { ".webp", true },
    };

    return extensions;
}

QString DataFactory::resolveTexturePath( const QString& basePath, bool isAnimated ) {
    QString fallback;

    for ( auto it = textureExtensions().constBegin(); it != textureExtensions().constEnd(); ++it ) {
        if ( it.value() != isAnimated ) {
            continue;
        }

        const QString path = basePath + it.key();
        if ( fallback.isEmpty() ) {
            fallback = path;
        }
        if ( QFile::exists( path ) ) {
            return path;
        }
    }

    return fallback;
}

AnimationModel DataFactory::loadAnimation( const QString& texturePath, bool isAnimated, int frameDurationMs ) {
    if ( !isAnimated ) {
        return AnimationModel( { QImage( texturePath ) }, frameDurationMs );
    }

    QImageReader reader( texturePath );

    QList<QImage> frames;
    while ( reader.canRead() ) {
        frames.append( reader.read() );
    }

    return AnimationModel( frames, frameDurationMs );
}

} // namespace Engine
