#include "ItemCategoryHelper.h"

namespace Engine {

std::string ItemCategoryHelper::toString( ItemCategoryEnum category ) {
    switch ( category ) {
    case ItemCategoryEnum::WEAPON:
        return "WEAPON";
    case ItemCategoryEnum::SHIELD:
        return "SHIELD";
    case ItemCategoryEnum::OFFHAND:
        return "OFFHAND";
    case ItemCategoryEnum::ARMOR:
        return "ARMOR";
    }

    return "";
}

ItemCategoryEnum ItemCategoryHelper::fromString( const std::string& value ) {
    if ( value == "WEAPON" ) {
        return ItemCategoryEnum::WEAPON;
    }
    if ( value == "SHIELD" ) {
        return ItemCategoryEnum::SHIELD;
    }
    if ( value == "OFFHAND" ) {
        return ItemCategoryEnum::OFFHAND;
    }
    if ( value == "ARMOR" ) {
        return ItemCategoryEnum::ARMOR;
    }

    return ItemCategoryEnum::UNKNOWN;
}

} // namespace Engine
