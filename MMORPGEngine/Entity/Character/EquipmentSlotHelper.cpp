#include "EquipmentSlotHelper.h"

namespace Engine {

std::string EquipmentSlotHelper::toString( EquipmentSlotEnum slot ) {
    switch ( slot ) {
    case EquipmentSlotEnum::NONE:
        return "NONE";
    case EquipmentSlotEnum::LEFT_HAND:
        return "LEFT_HAND";
    case EquipmentSlotEnum::RIGHT_HAND:
        return "RIGHT_HAND";
    case EquipmentSlotEnum::HEAD:
        return "HEAD";
    case EquipmentSlotEnum::CHEST:
        return "CHEST";
    case EquipmentSlotEnum::LEGS:
        return "LEGS";
    case EquipmentSlotEnum::FEET:
        return "FEET";
    case EquipmentSlotEnum::GLOVE:
        return "GLOVE";
    case EquipmentSlotEnum::NECK:
        return "NECK";
    case EquipmentSlotEnum::LEFT_RING:
        return "LEFT_RING";
    case EquipmentSlotEnum::RIGHT_RING:
        return "RIGHT_RING";
    }

    return "";
}

std::optional<EquipmentSlotEnum> EquipmentSlotHelper::fromString( const std::string& value ) {
    if ( value == "NONE" ) {
        return EquipmentSlotEnum::NONE;
    }
    if ( value == "LEFT_HAND" ) {
        return EquipmentSlotEnum::LEFT_HAND;
    }
    if ( value == "RIGHT_HAND" ) {
        return EquipmentSlotEnum::RIGHT_HAND;
    }
    if ( value == "HEAD" ) {
        return EquipmentSlotEnum::HEAD;
    }
    if ( value == "CHEST" ) {
        return EquipmentSlotEnum::CHEST;
    }
    if ( value == "LEGS" ) {
        return EquipmentSlotEnum::LEGS;
    }
    if ( value == "FEET" ) {
        return EquipmentSlotEnum::FEET;
    }
    if ( value == "GLOVE" ) {
        return EquipmentSlotEnum::GLOVE;
    }
    if ( value == "NECK" ) {
        return EquipmentSlotEnum::NECK;
    }
    if ( value == "LEFT_RING" ) {
        return EquipmentSlotEnum::LEFT_RING;
    }
    if ( value == "RIGHT_RING" ) {
        return EquipmentSlotEnum::RIGHT_RING;
    }

    return std::nullopt;
}

} // namespace Engine
