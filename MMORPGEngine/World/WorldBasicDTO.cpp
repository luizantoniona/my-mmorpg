#include "WorldBasicDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

WorldBasicDTO::WorldBasicDTO() :
    _worldName( "" ) {
}

WorldBasicDTO::~WorldBasicDTO() = default;

WorldBasicDTO WorldBasicDTO::fromModel( const WorldModel* world ) {
    WorldBasicDTO dto;

    dto._worldName = world->name().toStdString();

    return dto;
}

WorldBasicDTO WorldBasicDTO::fromJson( const Json::Value& json ) {
    WorldBasicDTO dto;

    if ( json.isMember( "worldName" ) && json[ "worldName" ].isString() ) {
        dto._worldName = json[ "worldName" ].asString();
    }

    return dto;
}

Json::Value WorldBasicDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::WORLD_BASIC );
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
