#include "GroundItemEventLeaveDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

GroundItemEventLeaveDTO::GroundItemEventLeaveDTO() :
    _idGroundItem( 0 ) {
}

GroundItemEventLeaveDTO::~GroundItemEventLeaveDTO() = default;

GroundItemEventLeaveDTO GroundItemEventLeaveDTO::fromJson( const Json::Value& json ) {
    GroundItemEventLeaveDTO dto;

    if ( json.isMember( "idGroundItem" ) && json[ "idGroundItem" ].isInt() ) {
        dto._idGroundItem = json[ "idGroundItem" ].asInt();
    }

    return dto;
}

Json::Value GroundItemEventLeaveDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::GROUND_ITEM_EVENT_LEAVE );
    json[ "idGroundItem" ] = _idGroundItem;

    return json;
}

int GroundItemEventLeaveDTO::idGroundItem() const {
    return _idGroundItem;
}

void GroundItemEventLeaveDTO::setIdGroundItem( int idGroundItem ) {
    _idGroundItem = idGroundItem;
}

} // namespace Engine
