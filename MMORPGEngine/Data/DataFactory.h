#ifndef DATAFACTORY_H
#define DATAFACTORY_H

#include <QString>

#include <MMORPGEngine/Data/Animation/AnimationModel.h>
#include <MMORPGEngine/Data/Object/ObjectCatalog.h>
#include <MMORPGEngine/Data/Tile/TileCatalog.h>

namespace Engine {

class DataFactory {
public:
    static void createObjectCatalog( const QString& configPath, ObjectCatalog& objectCatalog );

    static void createTileCatalog( const QString& configPath, TileCatalog& tileCatalog );
    static void saveTileCatalog( const QString& configPath, const TileCatalog& tileCatalog );

    static QString mapPath( const QString& configPath );

    static AnimationModel loadAnimation( const QString& texturePath, bool isAnimated, int frameDurationMs );
};

} // namespace Engine

#endif
