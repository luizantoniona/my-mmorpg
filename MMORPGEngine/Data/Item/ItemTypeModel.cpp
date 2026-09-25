#include "ItemTypeModel.h"

namespace Engine {

ItemTypeModel::ItemTypeModel() :
    _name( "" ),
    _handRequirement( std::nullopt ),
    _type( 0 ),
    _category( ItemCategoryEnum::UNKNOWN ),
    _slot( ItemSlotEnum::UNKNOWN ) {
}

uint32_t ItemTypeModel::type() const {
    return _type;
}

void ItemTypeModel::setType( uint32_t type ) {
    _type = type;
}

QString ItemTypeModel::name() const {
    return _name;
}

void ItemTypeModel::setName( const QString& name ) {
    _name = name;
}

ItemCategoryEnum ItemTypeModel::category() const {
    return _category;
}

void ItemTypeModel::setCategory( ItemCategoryEnum category ) {
    _category = category;
}

ItemSlotEnum ItemTypeModel::slot() const {
    return _slot;
}

void ItemTypeModel::setSlot( ItemSlotEnum slot ) {
    _slot = slot;
}

const std::optional<HandRequirementEnum>& ItemTypeModel::handRequirement() const {
    return _handRequirement;
}

void ItemTypeModel::setHandRequirement( HandRequirementEnum handRequirement ) {
    _handRequirement = handRequirement;
}

} // namespace Engine
