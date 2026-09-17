#include "EntityLeftDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

EntityLeftDTO::EntityLeftDTO() :
    _idCharacter( 0 ) {
}

EntityLeftDTO::~EntityLeftDTO() = default;

EntityLeftDTO EntityLeftDTO::fromJson( const Json::Value& json ) {
    EntityLeftDTO dto;

    if ( json.isMember( "idCharacter" ) && json[ "idCharacter" ].isInt() ) {
        dto._idCharacter = json[ "idCharacter" ].asInt();
    }

    return dto;
}

Json::Value EntityLeftDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::ENTITY_LEFT );
    json[ "idCharacter" ] = _idCharacter;

    return json;
}

int EntityLeftDTO::idCharacter() const {
    return _idCharacter;
}

void EntityLeftDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

} // namespace Engine
