#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include <QString>

#include <MMORPGEngine/Data/Object/ObjectCatalog.h>

namespace Engine {

class ObjectFactory {
public:
    static void createObjectCatalog( const QString& configPath, ObjectCatalog& objectCatalog );
    static void saveObjectCatalog( const QString& configPath, const ObjectCatalog& objectCatalog );
};

} // namespace Engine

#endif // OBJECTFACTORY_H
