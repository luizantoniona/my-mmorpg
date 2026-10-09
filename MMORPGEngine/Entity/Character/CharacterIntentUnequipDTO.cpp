#include "CharacterIntentUnequipDTO.h"

#include <MMORPGEngine/Entity/Character/EquipmentSlotHelper.h>
#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeHelper.h>

namespace Engine {

CharacterIntentUnequipDTO::CharacterIntentUnequipDTO() :
    _slot( std::nullopt ) {
}

CharacterIntentUnequipDTO::~CharacterIntentUnequipDTO() = default;

CharacterIntentUnequipDTO CharacterIntentUnequipDTO::fromJson( const Json::Value& json ) {
    CharacterIntentUnequipDTO dto;

    if ( json.isMember( "slot" ) && json[ "slot" ].isString() ) {
        dto._slot = EquipmentSlotHelper::fromString( json[ "slot" ].asString() );
    }

    return dto;
}

Json::Value CharacterIntentUnequipDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ClientMessageTypeHelper::toString( ClientMessageType::CHARACTER_INTENT_UNEQUIP );

    if ( _slot ) {
        json[ "slot" ] = EquipmentSlotHelper::toString( *_slot );
    }

    return json;
}

const std::optional<EquipmentSlotEnum>& CharacterIntentUnequipDTO::slot() const {
    return _slot;
}

void CharacterIntentUnequipDTO::setSlot( EquipmentSlotEnum slot ) {
    _slot = slot;
}

} // namespace Engine
