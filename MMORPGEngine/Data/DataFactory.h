#ifndef DATAFACTORY_H
#define DATAFACTORY_H

#include <QString>

#include <MMORPGEngine/Data/Animation/AnimationModel.h>

namespace Engine {

class DataFactory {
public:
    static QString mapPath( const QString& configPath );

    static AnimationModel loadAnimation( const QString& texturePath, bool isAnimated, int frameDurationMs );
};

} // namespace Engine

#endif // DATAFACTORY_H
