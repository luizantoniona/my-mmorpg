#ifndef CHARACTERFACTORY_H
#define CHARACTERFACTORY_H

#include <QString>

#include <MMORPGEngine/Data/Character/CharacterCatalog.h>

namespace Engine {

class CharacterFactory {
public:
    static bool isValidId( const QString& id );

    static void createCharacterCatalog( const QString& configPath, CharacterCatalog& characterCatalog );
};

} // namespace Engine

#endif // CHARACTERFACTORY_H
