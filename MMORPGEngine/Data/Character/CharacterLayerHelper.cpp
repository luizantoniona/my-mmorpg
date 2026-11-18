#include "CharacterLayerHelper.h"

namespace Engine {

std::string CharacterLayerHelper::toString( CharacterLayerEnum layer ) {
    switch ( layer ) {
    case CharacterLayerEnum::BODY:
        return "BODY";
    case CharacterLayerEnum::HEAD:
        return "HEAD";
    case CharacterLayerEnum::UNKNOWN:
        return "";
    }

    return "";
}

CharacterLayerEnum CharacterLayerHelper::fromString( const std::string& value ) {
    if ( value == "BODY" ) {
        return CharacterLayerEnum::BODY;
    }
    if ( value == "HEAD" ) {
        return CharacterLayerEnum::HEAD;
    }

    return CharacterLayerEnum::UNKNOWN;
}

const std::vector<CharacterLayerEnum>& CharacterLayerHelper::drawOrder() {
    static const std::vector<CharacterLayerEnum> order{
        CharacterLayerEnum::BODY,
        CharacterLayerEnum::HEAD,
    };

    return order;
}

} // namespace Engine
