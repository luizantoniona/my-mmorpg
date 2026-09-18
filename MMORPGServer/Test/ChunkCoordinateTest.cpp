#include <gtest/gtest.h>

#include <unordered_map>

#include <MMORPGServer/Server/Manager/ChunkCoordinate.h>

TEST( ChunkCoordinateTest, DefaultConstructor_IsOrigin ) {
    const Server::ChunkCoordinate coordinate;

    EXPECT_EQ( coordinate.x, 0 );
    EXPECT_EQ( coordinate.y, 0 );
    EXPECT_EQ( coordinate.z, 0 );
}

TEST( ChunkCoordinateTest, Equality_SameValues_AreEqual ) {
    const Server::ChunkCoordinate a( 1, 2, 3 );
    const Server::ChunkCoordinate b( 1, 2, 3 );

    EXPECT_TRUE( a == b );
}

TEST( ChunkCoordinateTest, Equality_DifferentValues_AreNotEqual ) {
    const Server::ChunkCoordinate a( 1, 2, 3 );
    const Server::ChunkCoordinate b( 1, 2, 4 );

    EXPECT_FALSE( a == b );
}

TEST( ChunkCoordinateTest, UsableAsUnorderedMapKey_RoundTrips ) {
    std::unordered_map<Server::ChunkCoordinate, int, Server::ChunkCoordinateHash> map;

    map[ Server::ChunkCoordinate( 1, 2, 3 ) ] = 42;

    ASSERT_NE( map.find( Server::ChunkCoordinate( 1, 2, 3 ) ), map.end() );
    EXPECT_EQ( map.find( Server::ChunkCoordinate( 1, 2, 3 ) )->second, 42 );
    EXPECT_EQ( map.find( Server::ChunkCoordinate( 3, 2, 1 ) ), map.end() );
}

TEST( ChunkCoordinateTest, Hash_SameCoordinate_ProducesSameHash ) {
    const Server::ChunkCoordinateHash hasher;
    const Server::ChunkCoordinate a( -1, 5, 2 );
    const Server::ChunkCoordinate b( -1, 5, 2 );

    EXPECT_EQ( hasher( a ), hasher( b ) );
}
