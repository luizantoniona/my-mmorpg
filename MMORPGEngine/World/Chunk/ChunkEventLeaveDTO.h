#ifndef CHUNKEVENTLEAVEDTO_H
#define CHUNKEVENTLEAVEDTO_H

#include <json/json.h>

namespace Engine {

class ChunkEventLeaveDTO {
public:
    ChunkEventLeaveDTO();
    ~ChunkEventLeaveDTO();

    static ChunkEventLeaveDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int chunkX() const;
    void setChunkX( int chunkX );

    int chunkY() const;
    void setChunkY( int chunkY );

private:
    int _chunkX;
    int _chunkY;
};

} // namespace Engine

#endif // CHUNKEVENTLEAVEDTO_H
