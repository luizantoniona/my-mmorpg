#ifndef CHARACTERAPPEARANCEFACTORY_H
#define CHARACTERAPPEARANCEFACTORY_H

#include <MMORPGEngine/Data/Appearance/AppearanceModel.h>
#include <MMORPGEngine/Data/Character/CharacterCatalog.h>
#include <MMORPGEngine/Entity/Character/CharacterAppearanceModel.h>

namespace Engine {

class CharacterAppearanceFactory {
public:
    static AppearanceModel createAppearance( const CharacterCatalog& characterCatalog, const CharacterAppearanceModel& characterAppearance );
};

} // namespace Engine

#endif // CHARACTERAPPEARANCEFACTORY_H
