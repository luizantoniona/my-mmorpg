#ifndef CHARACTERAPPEARANCEFEATUREHELPER_H
#define CHARACTERAPPEARANCEFEATUREHELPER_H

#include <string>
#include <vector>

#include <MMORPGEngine/Entity/Character/CharacterAppearanceFeatureEnum.h>

namespace Engine {

class CharacterAppearanceFeatureHelper {
public:
    static std::string toString( CharacterAppearanceFeatureEnum feature );
    static CharacterAppearanceFeatureEnum fromString( const std::string& value );

    static const std::vector<CharacterAppearanceFeatureEnum>& all();
};

} // namespace Engine

#endif // CHARACTERAPPEARANCEFEATUREHELPER_H
