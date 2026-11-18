#include "CreatureEventAttackDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

CreatureEventAttackDTO::CreatureEventAttackDTO() :
    _cells(),
    _idCreature( 0 ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

CreatureEventAttackDTO::~CreatureEventAttackDTO() = default;

CreatureEventAttackDTO CreatureEventAttackDTO::fromJson( const Json::Value& json ) {
    CreatureEventAttackDTO dto;

    if ( json.isMember( "idCreature" ) && json[ "idCreature" ].isInt() ) {
        dto._idCreature = json[ "idCreature" ].asInt();
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

Json::Value CreatureEventAttackDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::CREATURE_EVENT_ATTACK );
    json[ "idCreature" ] = _idCreature;
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

int CreatureEventAttackDTO::idCreature() const {
    return _idCreature;
}

void CreatureEventAttackDTO::setIdCreature( int idCreature ) {
    _idCreature = idCreature;
}

int CreatureEventAttackDTO::x() const {
    return _x;
}

void CreatureEventAttackDTO::setX( int x ) {
    _x = x;
}

int CreatureEventAttackDTO::y() const {
    return _y;
}

void CreatureEventAttackDTO::setY( int y ) {
    _y = y;
}

int CreatureEventAttackDTO::z() const {
    return _z;
}

void CreatureEventAttackDTO::setZ( int z ) {
    _z = z;
}

const std::vector<std::pair<int, int>>& CreatureEventAttackDTO::cells() const {
    return _cells;
}

void CreatureEventAttackDTO::setCells( const std::vector<std::pair<int, int>>& cells ) {
    _cells = cells;
}

} // namespace Engine
