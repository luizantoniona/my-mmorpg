#include "CreatureEventLeaveDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

CreatureEventLeaveDTO::CreatureEventLeaveDTO() :
    _idCreature( 0 ) {
}

CreatureEventLeaveDTO::~CreatureEventLeaveDTO() = default;

CreatureEventLeaveDTO CreatureEventLeaveDTO::fromJson( const Json::Value& json ) {
    CreatureEventLeaveDTO dto;

    if ( json.isMember( "idCreature" ) && json[ "idCreature" ].isInt() ) {
        dto._idCreature = json[ "idCreature" ].asInt();
    }

    return dto;
}

Json::Value CreatureEventLeaveDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::CREATURE_EVENT_LEAVE );
    json[ "idCreature" ] = _idCreature;

    return json;
}

int CreatureEventLeaveDTO::idCreature() const {
    return _idCreature;
}

void CreatureEventLeaveDTO::setIdCreature( int idCreature ) {
    _idCreature = idCreature;
}

} // namespace Engine
