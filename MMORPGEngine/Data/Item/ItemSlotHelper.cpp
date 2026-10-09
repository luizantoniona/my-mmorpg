#include "ItemSlotHelper.h"

namespace Engine {

std::string ItemSlotHelper::toString( ItemSlotEnum slot ) {
    switch ( slot ) {
    case ItemSlotEnum::HAND:
        return "HAND";
    case ItemSlotEnum::HEAD:
        return "HEAD";
    case ItemSlotEnum::CHEST:
        return "CHEST";
    case ItemSlotEnum::LEGS:
        return "LEGS";
    case ItemSlotEnum::FEET:
        return "FEET";
    case ItemSlotEnum::GLOVE:
        return "GLOVE";
    case ItemSlotEnum::RING:
        return "RING";
    case ItemSlotEnum::NECK:
        return "NECK";
    }

    return "";
}

ItemSlotEnum ItemSlotHelper::fromString( const std::string& value ) {
    if ( value == "HAND" ) {
        return ItemSlotEnum::HAND;
    }
    if ( value == "HEAD" ) {
        return ItemSlotEnum::HEAD;
    }
    if ( value == "CHEST" ) {
        return ItemSlotEnum::CHEST;
    }
    if ( value == "LEGS" ) {
        return ItemSlotEnum::LEGS;
    }
    if ( value == "FEET" ) {
        return ItemSlotEnum::FEET;
    }
    if ( value == "GLOVE" ) {
        return ItemSlotEnum::GLOVE;
    }
    if ( value == "RING" ) {
        return ItemSlotEnum::RING;
    }
    if ( value == "NECK" ) {
        return ItemSlotEnum::NECK;
    }

    return ItemSlotEnum::UNKNOWN;
}

} // namespace Engine
