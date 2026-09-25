#ifndef ITEMTYPEMODEL_H
#define ITEMTYPEMODEL_H

#include <cstdint>
#include <optional>

#include <QString>

#include <MMORPGEngine/Data/Item/HandRequirementEnum.h>
#include <MMORPGEngine/Data/Item/ItemCategoryEnum.h>
#include <MMORPGEngine/Data/Item/ItemSlotEnum.h>

namespace Engine {

class ItemTypeModel {
public:
    ItemTypeModel();

    uint32_t type() const;
    void setType( uint32_t type );

    QString name() const;
    void setName( const QString& name );

    ItemCategoryEnum category() const;
    void setCategory( ItemCategoryEnum category );

    ItemSlotEnum slot() const;
    void setSlot( ItemSlotEnum slot );

    const std::optional<HandRequirementEnum>& handRequirement() const;
    void setHandRequirement( HandRequirementEnum handRequirement );

private:
    QString _name;
    std::optional<HandRequirementEnum> _handRequirement;
    uint32_t _type;
    ItemCategoryEnum _category;
    ItemSlotEnum _slot;
};

} // namespace Engine

#endif // ITEMTYPEMODEL_H
