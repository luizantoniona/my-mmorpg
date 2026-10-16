#include "ChunkEventLeaveDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

ChunkEventLeaveDTO::ChunkEventLeaveDTO() :
    _chunkX( 0 ),
    _chunkY( 0 ) {
}

ChunkEventLeaveDTO::~ChunkEventLeaveDTO() = default;

ChunkEventLeaveDTO ChunkEventLeaveDTO::fromJson( const Json::Value& json ) {
    ChunkEventLeaveDTO dto;

    if ( json.isMember( "chunkX" ) && json[ "chunkX" ].isInt() ) {
        dto._chunkX = json[ "chunkX" ].asInt();
    }

    if ( json.isMember( "chunkY" ) && json[ "chunkY" ].isInt() ) {
        dto._chunkY = json[ "chunkY" ].asInt();
    }

    return dto;
}

Json::Value ChunkEventLeaveDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::CHUNK_EVENT_LEAVE );
    json[ "chunkX" ] = _chunkX;
    json[ "chunkY" ] = _chunkY;

    return json;
}

int ChunkEventLeaveDTO::chunkX() const {
    return _chunkX;
}

void ChunkEventLeaveDTO::setChunkX( int chunkX ) {
    _chunkX = chunkX;
}

int ChunkEventLeaveDTO::chunkY() const {
    return _chunkY;
}

void ChunkEventLeaveDTO::setChunkY( int chunkY ) {
    _chunkY = chunkY;
}

} // namespace Engine
