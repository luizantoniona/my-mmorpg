#include "DataFactory.h"

#include <QImageReader>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>

namespace Engine {

QString DataFactory::mapPath( const QString& configPath ) {
    Json::Value configJson = JsonHelper::loadJsonFile( configPath + "Config.json" );

    const QString mapFolder = QString( configJson[ "ActiveFolder" ].asCString() );

    return configPath + mapFolder + "/";
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
