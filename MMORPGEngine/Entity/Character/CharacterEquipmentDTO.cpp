#include "CharacterEquipmentDTO.h"

#include <MMORPGEngine/Entity/Character/EquipmentSlotHelper.h>

namespace Engine {

CharacterEquipmentDTO::CharacterEquipmentDTO() :
    _slot( EquipmentSlotEnum::NONE ),
    _idItem( 0 ) {
}

CharacterEquipmentDTO CharacterEquipmentDTO::fromModel( const CharacterEquipmentModel& equipment ) {
    CharacterEquipmentDTO dto;

    dto._slot = equipment.slot();
    dto._idItem = equipment.idItem();

    return dto;
}

CharacterEquipmentDTO CharacterEquipmentDTO::fromJson( const Json::Value& json ) {
    CharacterEquipmentDTO dto;

    if ( json.isMember( "slot" ) && json[ "slot" ].isString() ) {
        const std::optional<EquipmentSlotEnum> slot = EquipmentSlotHelper::fromString( json[ "slot" ].asString() );
        if ( slot.has_value() ) {
            dto._slot = slot.value();
        }
    }

    if ( json.isMember( "idItem" ) && json[ "idItem" ].isUInt() ) {
        dto._idItem = json[ "idItem" ].asUInt();
    }

    return dto;
}

Json::Value CharacterEquipmentDTO::toJson() const {
    Json::Value json;

    json[ "slot" ] = EquipmentSlotHelper::toString( _slot );
    json[ "idItem" ] = _idItem;

    return json;
}

EquipmentSlotEnum CharacterEquipmentDTO::slot() const {
    return _slot;
}

void CharacterEquipmentDTO::setSlot( EquipmentSlotEnum slot ) {
    _slot = slot;
}

uint32_t CharacterEquipmentDTO::idItem() const {
    return _idItem;
}

void CharacterEquipmentDTO::setIdItem( uint32_t idItem ) {
    _idItem = idItem;
}

} // namespace Engine
