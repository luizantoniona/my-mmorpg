#ifndef CHARACTERLAYERHELPER_H
#define CHARACTERLAYERHELPER_H

#include <string>
#include <vector>

#include <MMORPGEngine/Data/Character/CharacterLayerEnum.h>

namespace Engine {

class CharacterLayerHelper {
public:
    static std::string toString( CharacterLayerEnum layer );
    static CharacterLayerEnum fromString( const std::string& value );

    static const std::vector<CharacterLayerEnum>& drawOrder();
};

} // namespace Engine

#endif // CHARACTERLAYERHELPER_H
