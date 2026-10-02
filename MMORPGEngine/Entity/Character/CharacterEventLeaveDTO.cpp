#include "CharacterEventLeaveDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

CharacterEventLeaveDTO::CharacterEventLeaveDTO() :
    _idCharacter( 0 ) {
}

CharacterEventLeaveDTO::~CharacterEventLeaveDTO() = default;

CharacterEventLeaveDTO CharacterEventLeaveDTO::fromJson( const Json::Value& json ) {
    CharacterEventLeaveDTO dto;

    if ( json.isMember( "idCharacter" ) && json[ "idCharacter" ].isInt() ) {
        dto._idCharacter = json[ "idCharacter" ].asInt();
    }

    return dto;
}

Json::Value CharacterEventLeaveDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::CHARACTER_EVENT_LEAVE );
    json[ "idCharacter" ] = _idCharacter;

    return json;
}

int CharacterEventLeaveDTO::idCharacter() const {
    return _idCharacter;
}

void CharacterEventLeaveDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

} // namespace Engine
