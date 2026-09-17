#include "WorldBasicDTO.h"

namespace Engine {

WorldBasicDTO::WorldBasicDTO() :
    _worldName( "" ) {
}

WorldBasicDTO::~WorldBasicDTO() = default;

WorldBasicDTO WorldBasicDTO::fromJson( const Json::Value& json ) {
    WorldBasicDTO dto;

    if ( json.isMember( "worldName" ) && json[ "worldName" ].isString() ) {
        dto._worldName = json[ "worldName" ].asString();
    }

    return dto;
}

Json::Value WorldBasicDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = "world";
    json[ "worldName" ] = _worldName;

    return json;
}

std::string WorldBasicDTO::worldName() const {
    return _worldName;
}

void WorldBasicDTO::setWorldName( const std::string& worldName ) {
    _worldName = worldName;
}

} // namespace Engine
