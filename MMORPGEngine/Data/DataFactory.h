#ifndef DATAFACTORY_H
#define DATAFACTORY_H

#include <QString>

#include <MMORPGEngine/Data/Ground/GroundCatalog.h>

namespace Engine {

class DataFactory {
public:
    static void createGroundCatalog( const QString& groundsFile, GroundCatalog& groundCatalog );
};

} // namespace Engine

#endif
