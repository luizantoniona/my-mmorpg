#ifndef DATAFACTORY_H
#define DATAFACTORY_H

#include <QString>

#include <MMORPGEngine/Data/Object/ObjectTextureCatalog.h>
#include <MMORPGEngine/Data/Tile/TileTextureCatalog.h>

namespace Engine {

class DataFactory {
public:
    static void createTileTextureCatalog( const QString& configPath, TileTextureCatalog& tileTextureCatalog );
    static void createObjectTextureCatalog( const QString& configPath, ObjectTextureCatalog& objectTextureCatalog );
};

} // namespace Engine

#endif
