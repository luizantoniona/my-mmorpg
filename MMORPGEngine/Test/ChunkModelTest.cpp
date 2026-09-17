#include <gtest/gtest.h>

#include <MMORPGEngine/World/Chunk/ChunkModel.h>

TEST( ChunkModelTest, SetTile_ThenGet_ReturnsSameTileType ) {
    Engine::ChunkModel chunk;

    auto tile = std::make_unique<Engine::WorldTileModel>();
    tile->setTileType( 7 );
    chunk.setTile( 5, 10, 0, std::move( tile ) );

    ASSERT_NE( chunk.tile( 5, 10, 0 ), nullptr );
    EXPECT_EQ( chunk.tile( 5, 10, 0 )->tileType(), 7u );
}

TEST( ChunkModelTest, SetObject_ThenGet_ReturnsSameObjectType ) {
    Engine::ChunkModel chunk;

    auto object = std::make_unique<Engine::WorldObjectModel>();
    object->setObjectType( 3 );
    chunk.setObject( 5, 10, 0, std::move( object ) );

    ASSERT_NE( chunk.object( 5, 10, 0 ), nullptr );
    EXPECT_EQ( chunk.object( 5, 10, 0 )->objectType(), 3u );
}

TEST( ChunkModelTest, Tile_UnsetPosition_ReturnsNullptr ) {
    const Engine::ChunkModel chunk;

    EXPECT_EQ( chunk.tile( 0, 0, 0 ), nullptr );
}

TEST( ChunkModelTest, Object_UnsetPosition_ReturnsNullptr ) {
    const Engine::ChunkModel chunk;

    EXPECT_EQ( chunk.object( 0, 0, 0 ), nullptr );
}

TEST( ChunkModelTest, Tile_OutOfBoundsPositions_ReturnNullptr ) {
    Engine::ChunkModel chunk;

    EXPECT_EQ( chunk.tile( -1, 0, 0 ), nullptr );
    EXPECT_EQ( chunk.tile( 0, -1, 0 ), nullptr );
    EXPECT_EQ( chunk.tile( 32, 0, 0 ), nullptr );
    EXPECT_EQ( chunk.tile( 0, 32, 0 ), nullptr );
}

TEST( ChunkModelTest, SetTile_OutOfBoundsPosition_IsIgnored ) {
    Engine::ChunkModel chunk;

    auto tile = std::make_unique<Engine::WorldTileModel>();
    tile->setTileType( 7 );
    chunk.setTile( 32, 0, 0, std::move( tile ) );

    EXPECT_EQ( chunk.tile( 32, 0, 0 ), nullptr );
}

TEST( ChunkModelTest, DifferentZLayers_AreIndependent ) {
    Engine::ChunkModel chunk;

    auto groundTile = std::make_unique<Engine::WorldTileModel>();
    groundTile->setTileType( 1 );
    chunk.setTile( 5, 5, 0, std::move( groundTile ) );

    auto upperTile = std::make_unique<Engine::WorldTileModel>();
    upperTile->setTileType( 2 );
    chunk.setTile( 5, 5, 1, std::move( upperTile ) );

    ASSERT_NE( chunk.tile( 5, 5, 0 ), nullptr );
    ASSERT_NE( chunk.tile( 5, 5, 1 ), nullptr );
    EXPECT_EQ( chunk.tile( 5, 5, 0 )->tileType(), 1u );
    EXPECT_EQ( chunk.tile( 5, 5, 1 )->tileType(), 2u );
}
