#ifndef DATAFACTORY_H
#define DATAFACTORY_H

#include <QString>

#include <MMORPGEngine/Data/Object/ObjectCatalog.h>
#include <MMORPGEngine/Data/Tile/TileCatalog.h>

namespace Engine {

class DataFactory {
public:
    static void createObjectCatalog( const QString& configPath, ObjectCatalog& objectCatalog );
    static void createTileCatalog( const QString& configPath, TileCatalog& tileCatalog );
};

} // namespace Engine

#endif
