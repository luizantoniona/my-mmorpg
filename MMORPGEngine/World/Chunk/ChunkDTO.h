#ifndef CHUNKDTO_H
#define CHUNKDTO_H

#include <cstdint>
#include <vector>

#include <json/json.h>

#include <MMORPGEngine/World/WorldModel.h>

namespace Engine {

class ChunkDTO {
public:
    class Cell {
    public:
        Cell( int x, int y, uint32_t type );

        int x;
        int y;
        uint32_t type;
    };

    class Floor {
    public:
        Floor();

        int z;
        std::vector<uint32_t> tiles;
        std::vector<Cell> overlays;
        std::vector<Cell> objects;
    };

    ChunkDTO();
    ~ChunkDTO();

    static ChunkDTO fromModel( const WorldModel& world, int chunkX, int chunkY );
    static ChunkDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    void applyTo( WorldModel& world ) const;

    int chunkX() const;
    void setChunkX( int chunkX );

    int chunkY() const;
    void setChunkY( int chunkY );

    const std::vector<Floor>& floors() const;
    void addFloor( const Floor& floor );

private:
    std::vector<Floor> _floors;
    int _chunkX;
    int _chunkY;
};

} // namespace Engine

#endif // CHUNKDTO_H
