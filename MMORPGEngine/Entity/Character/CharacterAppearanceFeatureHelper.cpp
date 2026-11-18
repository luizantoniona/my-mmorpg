#include "CharacterAppearanceFeatureHelper.h"

namespace Engine {

std::string CharacterAppearanceFeatureHelper::toString( CharacterAppearanceFeatureEnum feature ) {
    switch ( feature ) {
    case CharacterAppearanceFeatureEnum::BODY:
        return "BODY";
    case CharacterAppearanceFeatureEnum::UNKNOWN:
        return "";
    }

    return "";
}

CharacterAppearanceFeatureEnum CharacterAppearanceFeatureHelper::fromString( const std::string& value ) {
    if ( value == "BODY" ) {
        return CharacterAppearanceFeatureEnum::BODY;
    }

    return CharacterAppearanceFeatureEnum::UNKNOWN;
}

const std::vector<CharacterAppearanceFeatureEnum>& CharacterAppearanceFeatureHelper::all() {
    static const std::vector<CharacterAppearanceFeatureEnum> features{
        CharacterAppearanceFeatureEnum::BODY,
    };

    return features;
}

} // namespace Engine
