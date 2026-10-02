#ifndef ITEMTYPEMODEL_H
#define ITEMTYPEMODEL_H

#include <optional>

#include <QString>

#include <MMORPGEngine/Data/Item/HandRequirementEnum.h>
#include <MMORPGEngine/Data/Item/ItemCategoryEnum.h>
#include <MMORPGEngine/Data/Item/ItemSlotEnum.h>

namespace Engine {

class ItemTypeModel {
public:
    ItemTypeModel();

    QString type() const;
    void setType( const QString& type );

    QString name() const;
    void setName( const QString& name );

    ItemCategoryEnum category() const;
    void setCategory( ItemCategoryEnum category );

    ItemSlotEnum slot() const;
    void setSlot( ItemSlotEnum slot );

    const std::optional<HandRequirementEnum>& handRequirement() const;
    void setHandRequirement( HandRequirementEnum handRequirement );

private:
    QString _type;
    QString _name;
    std::optional<HandRequirementEnum> _handRequirement;
    ItemCategoryEnum _category;
    ItemSlotEnum _slot;
};

} // namespace Engine

#endif // ITEMTYPEMODEL_H
