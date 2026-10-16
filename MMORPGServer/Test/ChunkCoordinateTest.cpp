#include <gtest/gtest.h>

#include <string>
#include <unordered_map>

#include <MMORPGServer/Server/Manager/ChunkCoordinate.h>

TEST( ChunkCoordinateTest, DefaultConstructor_IsOrigin ) {
    const Server::ChunkCoordinate coordinate;

    EXPECT_EQ( coordinate.x, 0 );
    EXPECT_EQ( coordinate.y, 0 );
    EXPECT_EQ( coordinate.z, 0 );
}

namespace {

class EqualityCase {
public:
    EqualityCase( std::string name, Server::ChunkCoordinate left, Server::ChunkCoordinate right, bool expected ) :
        name( std::move( name ) ),
        left( left ),
        right( right ),
        expected( expected ) {
    }

    std::string name;
    Server::ChunkCoordinate left;
    Server::ChunkCoordinate right;
    bool expected;
};

class ChunkCoordinateEqualityTest : public ::testing::TestWithParam<EqualityCase> {};

} // namespace

TEST_P( ChunkCoordinateEqualityTest, Equality_ReturnsExpected ) {
    EXPECT_EQ( GetParam().left == GetParam().right, GetParam().expected );
}

INSTANTIATE_TEST_SUITE_P(
    Coordinates,
    ChunkCoordinateEqualityTest,
    ::testing::Values(
        EqualityCase( "SameValues_AreEqual", Server::ChunkCoordinate( 1, 2, 3 ), Server::ChunkCoordinate( 1, 2, 3 ), true ),
        EqualityCase( "DifferentX_AreNotEqual", Server::ChunkCoordinate( 1, 2, 3 ), Server::ChunkCoordinate( 0, 2, 3 ), false ),
        EqualityCase( "DifferentY_AreNotEqual", Server::ChunkCoordinate( 1, 2, 3 ), Server::ChunkCoordinate( 1, 0, 3 ), false ),
        EqualityCase( "DifferentFloor_AreNotEqual", Server::ChunkCoordinate( 1, 2, 3 ), Server::ChunkCoordinate( 1, 2, 4 ), false ) ),
    []( const ::testing::TestParamInfo<EqualityCase>& info ) { return info.param.name; } );

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
