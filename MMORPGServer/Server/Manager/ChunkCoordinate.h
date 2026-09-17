#ifndef CHUNKCOORDINATE_H
#define CHUNKCOORDINATE_H

#include <cstddef>

namespace Server {

class ChunkCoordinate {
public:
    ChunkCoordinate();
    ChunkCoordinate( int x, int y, int z );

    bool operator==( const ChunkCoordinate& other ) const;

    int x;
    int y;
    int z;
};

class ChunkCoordinateHash {
public:
    std::size_t operator()( const ChunkCoordinate& coordinate ) const;
};

} // namespace Server

#endif // CHUNKCOORDINATE_H
