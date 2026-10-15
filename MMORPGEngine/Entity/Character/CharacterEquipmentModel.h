#ifndef CHARACTEREQUIPMENTMODEL_H
#define CHARACTEREQUIPMENTMODEL_H

#include <QString>

#include <MMORPGEngine/Entity/Character/EquipmentSlotEnum.h>

namespace Engine {

class CharacterEquipmentModel {
public:
    CharacterEquipmentModel();

    EquipmentSlotEnum slot() const;
    void setSlot( EquipmentSlotEnum slot );

    QString idItem() const;
    void setIdItem( const QString& idItem );

private:
    QString _idItem;
    EquipmentSlotEnum _slot;
};

} // namespace Engine

#endif // CHARACTEREQUIPMENTMODEL_H
