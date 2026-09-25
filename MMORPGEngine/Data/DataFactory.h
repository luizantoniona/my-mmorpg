#ifndef DATAFACTORY_H
#define DATAFACTORY_H

#include <QMap>
#include <QString>

#include <MMORPGEngine/Data/Animation/AnimationModel.h>

namespace Engine {

class DataFactory {
public:
    static QString mapPath( const QString& configPath );

    static const QMap<QString, bool>& textureExtensions();
    static QString resolveTexturePath( const QString& basePath, bool isAnimated );

    static AnimationModel loadAnimation( const QString& texturePath, bool isAnimated, int frameDurationMs );
};

} // namespace Engine

#endif // DATAFACTORY_H
