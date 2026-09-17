#include "ChunkCoordinate.h"

#include <functional>

namespace Server {

ChunkCoordinate::ChunkCoordinate() :
    x( 0 ),
    y( 0 ),
    z( 0 ) {
}

ChunkCoordinate::ChunkCoordinate( int x, int y, int z ) :
    x( x ),
    y( y ),
    z( z ) {
}

bool ChunkCoordinate::operator==( const ChunkCoordinate& other ) const {
    return x == other.x && y == other.y && z == other.z;
}

std::size_t ChunkCoordinateHash::operator()( const ChunkCoordinate& coordinate ) const {
    const std::size_t hashX = std::hash<int>()( coordinate.x );
    const std::size_t hashY = std::hash<int>()( coordinate.y );
    const std::size_t hashZ = std::hash<int>()( coordinate.z );

    return ( ( hashX * 397 ) ^ hashY ) * 397 ^ hashZ;
}

} // namespace Server
