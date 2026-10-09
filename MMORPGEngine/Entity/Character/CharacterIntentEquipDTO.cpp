#include "CharacterIntentEquipDTO.h"

#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeHelper.h>

namespace Engine {

CharacterIntentEquipDTO::CharacterIntentEquipDTO() :
    _position( std::nullopt ) {
}

CharacterIntentEquipDTO::~CharacterIntentEquipDTO() = default;

CharacterIntentEquipDTO CharacterIntentEquipDTO::fromJson( const Json::Value& json ) {
    CharacterIntentEquipDTO dto;

    if ( json.isMember( "position" ) && json[ "position" ].isUInt() ) {
        dto._position = json[ "position" ].asUInt();
    }

    return dto;
}

Json::Value CharacterIntentEquipDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ClientMessageTypeHelper::toString( ClientMessageType::CHARACTER_INTENT_EQUIP );

    if ( _position ) {
        json[ "position" ] = *_position;
    }

    return json;
}

const std::optional<uint32_t>& CharacterIntentEquipDTO::position() const {
    return _position;
}

void CharacterIntentEquipDTO::setPosition( uint32_t position ) {
    _position = position;
}

} // namespace Engine
