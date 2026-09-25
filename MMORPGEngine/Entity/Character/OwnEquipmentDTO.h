#ifndef OWNEQUIPMENTDTO_H
#define OWNEQUIPMENTDTO_H

#include <vector>

#include <json/json.h>

#include <MMORPGEngine/Entity/Character/CharacterEquipmentDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterModel.h>

namespace Engine {

class OwnEquipmentDTO {
public:
    OwnEquipmentDTO();

    static OwnEquipmentDTO fromModel( const CharacterModel* character );
    static OwnEquipmentDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

    const std::vector<CharacterEquipmentDTO>& equipment() const;
    void setEquipment( const std::vector<CharacterEquipmentDTO>& equipment );

private:
    std::vector<CharacterEquipmentDTO> _equipment;
    int _idCharacter;
};

} // namespace Engine

#endif // OWNEQUIPMENTDTO_H
