#include "CharacterIntentPickupDTO.h"

#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeHelper.h>

namespace Engine {

CharacterIntentPickupDTO::CharacterIntentPickupDTO() :
    _idGroundItem( std::nullopt ) {
}

CharacterIntentPickupDTO::~CharacterIntentPickupDTO() = default;

CharacterIntentPickupDTO CharacterIntentPickupDTO::fromJson( const Json::Value& json ) {
    CharacterIntentPickupDTO dto;

    if ( json.isMember( "idGroundItem" ) && json[ "idGroundItem" ].isInt() ) {
        dto._idGroundItem = json[ "idGroundItem" ].asInt();
    }

    return dto;
}

Json::Value CharacterIntentPickupDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ClientMessageTypeHelper::toString( ClientMessageType::CHARACTER_INTENT_PICKUP );

    if ( _idGroundItem ) {
        json[ "idGroundItem" ] = *_idGroundItem;
    }

    return json;
}

const std::optional<int>& CharacterIntentPickupDTO::idGroundItem() const {
    return _idGroundItem;
}

void CharacterIntentPickupDTO::setIdGroundItem( int idGroundItem ) {
    _idGroundItem = idGroundItem;
}

} // namespace Engine
