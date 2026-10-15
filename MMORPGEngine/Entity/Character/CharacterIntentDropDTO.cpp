#include "CharacterIntentDropDTO.h"

#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeHelper.h>

namespace Engine {

CharacterIntentDropDTO::CharacterIntentDropDTO() :
    _position( std::nullopt ) {
}

CharacterIntentDropDTO::~CharacterIntentDropDTO() = default;

CharacterIntentDropDTO CharacterIntentDropDTO::fromJson( const Json::Value& json ) {
    CharacterIntentDropDTO dto;

    if ( json.isMember( "position" ) && json[ "position" ].isUInt() ) {
        dto._position = json[ "position" ].asUInt();
    }

    return dto;
}

Json::Value CharacterIntentDropDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ClientMessageTypeHelper::toString( ClientMessageType::CHARACTER_INTENT_DROP );

    if ( _position ) {
        json[ "position" ] = *_position;
    }

    return json;
}

const std::optional<uint32_t>& CharacterIntentDropDTO::position() const {
    return _position;
}

void CharacterIntentDropDTO::setPosition( uint32_t position ) {
    _position = position;
}

} // namespace Engine
