#include <gtest/gtest.h>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Tile/TileModel.h>
#include <MMORPGEngine/World/WorldModel.h>

TEST( WorldModelTest, Floors_Empty_ReturnsEmptyVector ) {
    Engine::WorldModel world;

    EXPECT_TRUE( world.floors().empty() );
}

TEST( WorldModelTest, AddFloor_DuplicateValues_AreDeduplicated ) {
    Engine::WorldModel world;

    world.addFloor( 0 );
    world.addFloor( 0 );

    EXPECT_EQ( world.floors().size(), 1u );
}

TEST( WorldModelTest, AddFloor_OutOfOrderValues_AreReturnedSorted ) {
    Engine::WorldModel world;

    world.addFloor( 2 );
    world.addFloor( -1 );
    world.addFloor( 0 );

    const std::vector<int> expected{ -1, 0, 2 };
    EXPECT_EQ( world.floors(), expected );
}

TEST( WorldModelTest, Chunk_SameCoordinatesTwice_ReturnsSamePointer ) {
    Engine::WorldModel world;

    Engine::ChunkModel* first = world.chunk( 1, 2 );
    Engine::ChunkModel* second = world.chunk( 1, 2 );

    EXPECT_EQ( first, second );
}

TEST( WorldModelTest, Chunk_DifferentCoordinates_ReturnDifferentPointers ) {
    Engine::WorldModel world;

    Engine::ChunkModel* first = world.chunk( 0, 0 );
    Engine::ChunkModel* second = world.chunk( 1, 0 );

    EXPECT_NE( first, second );
}

TEST( WorldModelTest, ConstChunk_NotYetCreated_ReturnsNullptr ) {
    const Engine::WorldModel world;

    EXPECT_EQ( world.chunk( 0, 0 ), nullptr );
}

TEST( WorldModelTest, Tile_NoChunkLoaded_ReturnsNullptr ) {
    const Engine::WorldModel world;

    EXPECT_EQ( world.tile( 0, 0, 0 ), nullptr );
}

TEST( WorldModelTest, Object_NoChunkLoaded_ReturnsNullptr ) {
    const Engine::WorldModel world;

    EXPECT_EQ( world.object( 0, 0, 0 ), nullptr );
}

TEST( WorldModelTest, Tile_NegativeCoordinates_ReturnsNullptrEvenWhenChunkExists ) {
    Engine::WorldModel world;

    world.chunk( 0, 0 );

    EXPECT_EQ( world.tile( -1, -1, 0 ), nullptr );
}

TEST( WorldModelTest, SetTile_TypeZero_ClearsExistingTile ) {
    Engine::TileModel tile;
    tile.setType( 9201 );
    tile.setName( "WorldModelTestTile" );
    Engine::Singleton<Engine::DataManager>::instance().addTile( tile );

    Engine::WorldModel world;
    world.setTile( 0, 0, 0, 9201 );
    ASSERT_NE( world.tile( 0, 0, 0 ), nullptr );
    ASSERT_NE( world.tile( 0, 0, 0 )->tileModel(), nullptr );

    world.setTile( 0, 0, 0, 0 );

    ASSERT_NE( world.tile( 0, 0, 0 ), nullptr );
    EXPECT_EQ( world.tile( 0, 0, 0 )->tileModel(), nullptr );
    EXPECT_EQ( world.tile( 0, 0, 0 )->tileType(), 0u );
}

TEST( WorldModelTest, SetTile_UnknownType_DoesNotCrash ) {
    Engine::WorldModel world;

    EXPECT_NO_THROW( world.setTile( 0, 0, 0, 999999 ) );
}
