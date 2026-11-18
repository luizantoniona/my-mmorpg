#include "AttackStrikeDTO.h"

namespace Engine {

AttackStrikeDTO::AttackStrikeDTO() :
    _cells(),
    _seconds( 0.0 ) {
}

AttackStrikeDTO::AttackStrikeDTO( double seconds, std::vector<std::pair<int, int>> cells ) :
    _cells( std::move( cells ) ),
    _seconds( seconds ) {
}

AttackStrikeDTO AttackStrikeDTO::fromJson( const Json::Value& json ) {
    AttackStrikeDTO dto;

    if ( json.isMember( "seconds" ) && json[ "seconds" ].isNumeric() ) {
        dto._seconds = json[ "seconds" ].asDouble();
    }

    if ( json.isMember( "cells" ) && json[ "cells" ].isArray() ) {
        for ( const Json::Value& cellJson : json[ "cells" ] ) {
            if ( cellJson.isMember( "x" ) && cellJson[ "x" ].isInt() && cellJson.isMember( "y" ) && cellJson[ "y" ].isInt() ) {
                dto._cells.emplace_back( cellJson[ "x" ].asInt(), cellJson[ "y" ].asInt() );
            }
        }
    }

    return dto;
}

Json::Value AttackStrikeDTO::toJson() const {
    Json::Value json;

    json[ "seconds" ] = _seconds;
    json[ "cells" ] = Json::Value( Json::arrayValue );

    for ( const std::pair<int, int>& cell : _cells ) {
        Json::Value cellJson;
        cellJson[ "x" ] = cell.first;
        cellJson[ "y" ] = cell.second;
        json[ "cells" ].append( cellJson );
    }

    return json;
}

double AttackStrikeDTO::seconds() const {
    return _seconds;
}

const std::vector<std::pair<int, int>>& AttackStrikeDTO::cells() const {
    return _cells;
}

} // namespace Engine
