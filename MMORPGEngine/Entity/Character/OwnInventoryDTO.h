#ifndef OWNINVENTORYDTO_H
#define OWNINVENTORYDTO_H

#include <vector>

#include <json/json.h>

#include <MMORPGEngine/Entity/Character/CharacterInventoryDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterModel.h>

namespace Engine {

class OwnInventoryDTO {
public:
    OwnInventoryDTO();

    static OwnInventoryDTO fromModel( const CharacterModel* character );
    static OwnInventoryDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

    const std::vector<CharacterInventoryDTO>& items() const;
    void setItems( const std::vector<CharacterInventoryDTO>& items );

private:
    std::vector<CharacterInventoryDTO> _items;
    int _idCharacter;
};

} // namespace Engine

#endif // OWNINVENTORYDTO_H
