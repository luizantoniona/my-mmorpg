#include "CreatureLeftDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

CreatureLeftDTO::CreatureLeftDTO() :
    _idCreature( 0 ) {
}

CreatureLeftDTO::~CreatureLeftDTO() = default;

CreatureLeftDTO CreatureLeftDTO::fromJson( const Json::Value& json ) {
    CreatureLeftDTO dto;

    if ( json.isMember( "idCreature" ) && json[ "idCreature" ].isInt() ) {
        dto._idCreature = json[ "idCreature" ].asInt();
    }

    return dto;
}

Json::Value CreatureLeftDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::CREATURE_LEFT );
    json[ "idCreature" ] = _idCreature;

    return json;
}

int CreatureLeftDTO::idCreature() const {
    return _idCreature;
}

void CreatureLeftDTO::setIdCreature( int idCreature ) {
    _idCreature = idCreature;
}

} // namespace Engine
