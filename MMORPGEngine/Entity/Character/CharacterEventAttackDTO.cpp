#include "CharacterEventAttackDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

CharacterEventAttackDTO::CharacterEventAttackDTO() :
    _cells(),
    _idCharacter( 0 ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

CharacterEventAttackDTO::~CharacterEventAttackDTO() = default;

CharacterEventAttackDTO CharacterEventAttackDTO::fromJson( const Json::Value& json ) {
    CharacterEventAttackDTO dto;

    if ( json.isMember( "idCharacter" ) && json[ "idCharacter" ].isInt() ) {
        dto._idCharacter = json[ "idCharacter" ].asInt();
    }

    if ( json.isMember( "x" ) && json[ "x" ].isInt() ) {
        dto._x = json[ "x" ].asInt();
    }

    if ( json.isMember( "y" ) && json[ "y" ].isInt() ) {
        dto._y = json[ "y" ].asInt();
    }

    if ( json.isMember( "z" ) && json[ "z" ].isInt() ) {
        dto._z = json[ "z" ].asInt();
    }

    if ( json.isMember( "cells" ) && json[ "cells" ].isArray() ) {
        for ( const Json::Value& cellJson : json[ "cells" ] ) {
            if ( cellJson.isMember( "x" ) && cellJson[ "x" ].isInt() && cellJson.isMember( "y" ) && cellJson[ "y" ].isInt() ) {
                dto._cells.emplace_back( cellJson[ "x" ].asInt(), cellJson[ "y" ].asInt() );
            }
        }
    }

    if ( dto._cells.empty() ) {
        dto._cells.emplace_back( dto._x, dto._y );
    }

    return dto;
}

Json::Value CharacterEventAttackDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::CHARACTER_EVENT_ATTACK );
    json[ "idCharacter" ] = _idCharacter;
    json[ "x" ] = _x;
    json[ "y" ] = _y;
    json[ "z" ] = _z;

    json[ "cells" ] = Json::Value( Json::arrayValue );
    for ( const std::pair<int, int>& cell : _cells ) {
        Json::Value cellJson;
        cellJson[ "x" ] = cell.first;
        cellJson[ "y" ] = cell.second;
        json[ "cells" ].append( cellJson );
    }

    return json;
}

int CharacterEventAttackDTO::idCharacter() const {
    return _idCharacter;
}

void CharacterEventAttackDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

int CharacterEventAttackDTO::x() const {
    return _x;
}

void CharacterEventAttackDTO::setX( int x ) {
    _x = x;
}

int CharacterEventAttackDTO::y() const {
    return _y;
}

void CharacterEventAttackDTO::setY( int y ) {
    _y = y;
}

int CharacterEventAttackDTO::z() const {
    return _z;
}

void CharacterEventAttackDTO::setZ( int z ) {
    _z = z;
}

const std::vector<std::pair<int, int>>& CharacterEventAttackDTO::cells() const {
    return _cells;
}

void CharacterEventAttackDTO::setCells( const std::vector<std::pair<int, int>>& cells ) {
    _cells = cells;
}

} // namespace Engine
