#ifndef CHARACTEREQUIPMENTDTO_H
#define CHARACTEREQUIPMENTDTO_H

#include <json/json.h>

#include <QString>

#include <MMORPGEngine/Entity/Character/CharacterEquipmentModel.h>

namespace Engine {

class CharacterEquipmentDTO {
public:
    CharacterEquipmentDTO();

    static CharacterEquipmentDTO fromModel( const CharacterEquipmentModel& equipment );
    static CharacterEquipmentDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    EquipmentSlotEnum slot() const;
    void setSlot( EquipmentSlotEnum slot );

    QString idItem() const;
    void setIdItem( const QString& idItem );

private:
    QString _idItem;
    EquipmentSlotEnum _slot;
};

} // namespace Engine

#endif // CHARACTEREQUIPMENTDTO_H
