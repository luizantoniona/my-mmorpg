#ifndef TILEFACTORY_H
#define TILEFACTORY_H

#include <QString>

#include <MMORPGEngine/Data/Tile/TileCatalog.h>

namespace Engine {

class TileFactory {
public:
    static void createTileCatalog( const QString& configPath, TileCatalog& tileCatalog );
    static void saveTileCatalog( const QString& configPath, const TileCatalog& tileCatalog );
};

} // namespace Engine

#endif // TILEFACTORY_H
