#include "ItemSlotHelper.h"

namespace Engine {

std::string ItemSlotHelper::toString( ItemSlotEnum slot ) {
    switch ( slot ) {
    case ItemSlotEnum::HAND:
        return "Hand";
    case ItemSlotEnum::HEAD:
        return "Head";
    case ItemSlotEnum::CHEST:
        return "Chest";
    case ItemSlotEnum::LEGS:
        return "Legs";
    case ItemSlotEnum::FEET:
        return "Feet";
    case ItemSlotEnum::GLOVE:
        return "Glove";
    case ItemSlotEnum::RING:
        return "Ring";
    case ItemSlotEnum::NECK:
        return "Neck";
    }

    return "";
}

ItemSlotEnum ItemSlotHelper::fromString( const std::string& value ) {
    if ( value == "Hand" ) {
        return ItemSlotEnum::HAND;
    }
    if ( value == "Head" ) {
        return ItemSlotEnum::HEAD;
    }
    if ( value == "Chest" ) {
        return ItemSlotEnum::CHEST;
    }
    if ( value == "Legs" ) {
        return ItemSlotEnum::LEGS;
    }
    if ( value == "Feet" ) {
        return ItemSlotEnum::FEET;
    }
    if ( value == "Glove" ) {
        return ItemSlotEnum::GLOVE;
    }
    if ( value == "Ring" ) {
        return ItemSlotEnum::RING;
    }
    if ( value == "Neck" ) {
        return ItemSlotEnum::NECK;
    }

    return ItemSlotEnum::UNKNOWN;
}

} // namespace Engine
