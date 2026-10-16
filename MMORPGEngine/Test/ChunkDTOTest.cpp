#include <gtest/gtest.h>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Object/ObjectModel.h>
#include <MMORPGEngine/Data/Tile/TileModel.h>
#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>
#include <MMORPGEngine/World/Chunk/ChunkDTO.h>
#include <MMORPGEngine/World/Chunk/ChunkEventLeaveDTO.h>
#include <MMORPGEngine/World/WorldConstants.h>
#include <MMORPGEngine/World/WorldModel.h>

namespace {

constexpr uint32_t GROUND_TILE = 9701;
constexpr uint32_t OVERLAY_TILE = 9702;
constexpr uint32_t TREE_OBJECT = 9703;
constexpr int CHUNK_SIZE = Engine::WorldConstants::CHUNK_SIZE;

void registerTypes() {
    Engine::DataManager& dataManager = Engine::Singleton<Engine::DataManager>::instance();

    Engine::TileModel ground;
    ground.setType( GROUND_TILE );
    dataManager.addTile( ground );

    Engine::TileModel overlay;
    overlay.setType( OVERLAY_TILE );
    dataManager.addTile( overlay );

    Engine::ObjectModel tree;
    tree.setType( TREE_OBJECT );
    dataManager.addObject( tree );
}

Engine::WorldModel makeWorld() {
    registerTypes();

    Engine::WorldModel world;
    world.addFloor( 0 );
    world.addFloor( 1 );

    world.setTile( CHUNK_SIZE + 2, 3, 0, GROUND_TILE );
    world.setOverlay( CHUNK_SIZE + 2, 3, 0, OVERLAY_TILE );
    world.setObject( CHUNK_SIZE + 5, 6, 0, TREE_OBJECT );
    world.setTile( CHUNK_SIZE + 7, 8, 1, GROUND_TILE );

    return world;
}

} // namespace

TEST( ChunkDTOTest, FromModel_ThenJsonThenApply_ReproducesTilesOverlaysAndObjects ) {
    const Engine::WorldModel source = makeWorld();

    const Json::Value json = Engine::ChunkDTO::fromModel( source, 1, 0 ).toJson();
    const Engine::ChunkDTO reloaded = Engine::ChunkDTO::fromJson( json );

    Engine::WorldModel target;
    reloaded.applyTo( target );

    EXPECT_EQ( Engine::ServerMessageTypeHelper::fromMessage( json ), Engine::ServerMessageType::CHUNK );
    EXPECT_EQ( reloaded.chunkX(), 1 );
    EXPECT_EQ( reloaded.chunkY(), 0 );
    ASSERT_NE( target.tile( CHUNK_SIZE + 2, 3, 0 ), nullptr );
    EXPECT_EQ( target.tile( CHUNK_SIZE + 2, 3, 0 )->tileType(), GROUND_TILE );
    EXPECT_EQ( target.tile( CHUNK_SIZE + 2, 3, 0 )->overlayType(), OVERLAY_TILE );
    ASSERT_NE( target.object( CHUNK_SIZE + 5, 6, 0 ), nullptr );
    EXPECT_EQ( target.object( CHUNK_SIZE + 5, 6, 0 )->objectType(), TREE_OBJECT );
    ASSERT_NE( target.tile( CHUNK_SIZE + 7, 8, 1 ), nullptr );
    EXPECT_EQ( target.tile( CHUNK_SIZE + 7, 8, 1 )->tileType(), GROUND_TILE );
    EXPECT_EQ( target.floors(), std::vector<int>( { 0, 1 } ) );
}

TEST( ChunkDTOTest, FromModel_ChunkNotLoaded_HasNoFloors ) {
    const Engine::WorldModel source = makeWorld();

    EXPECT_TRUE( Engine::ChunkDTO::fromModel( source, 5, 5 ).floors().empty() );
}

TEST( ChunkDTOTest, FromModel_SkipsFloorsWithoutContent ) {
    Engine::WorldModel source = makeWorld();
    source.addFloor( 2 );

    const Engine::ChunkDTO chunk = Engine::ChunkDTO::fromModel( source, 1, 0 );

    ASSERT_EQ( chunk.floors().size(), 2u );
    EXPECT_EQ( chunk.floors().front().z, 0 );
    EXPECT_EQ( chunk.floors().back().z, 1 );
}

TEST( ChunkDTOTest, FromModel_ChunkWithOnlyEmptyAllocatedCells_HasNoFloors ) {
    registerTypes();

    Engine::WorldModel source;
    source.addFloor( 0 );
    source.chunk( 0, 0 )->tile( 3, 3, 0 );
    source.chunk( 0, 0 )->object( 4, 4, 0 );

    EXPECT_TRUE( Engine::ChunkDTO::fromModel( source, 0, 0 ).floors().empty() );
}

TEST( ChunkDTOTest, FromModel_ThenApply_OverlayWithoutBaseTile_IsPreserved ) {
    registerTypes();

    Engine::WorldModel source;
    source.addFloor( 0 );
    source.setOverlay( 3, 4, 0, OVERLAY_TILE );

    Engine::WorldModel target;
    Engine::ChunkDTO::fromJson( Engine::ChunkDTO::fromModel( source, 0, 0 ).toJson() ).applyTo( target );

    ASSERT_NE( target.tile( 3, 4, 0 ), nullptr );
    EXPECT_EQ( target.tile( 3, 4, 0 )->tileType(), 0u );
    EXPECT_EQ( target.tile( 3, 4, 0 )->overlayType(), OVERLAY_TILE );
}

TEST( ChunkDTOTest, Apply_UnknownTileTypeInCatalog_IsIgnoredWithoutBreakingTheRest ) {
    registerTypes();

    Engine::ChunkDTO::Floor floor;
    floor.tiles.assign( CHUNK_SIZE * CHUNK_SIZE, 0 );
    floor.tiles[ 0 ] = 999999;
    floor.tiles[ 1 ] = GROUND_TILE;

    Engine::ChunkDTO chunk;
    chunk.addFloor( floor );

    Engine::WorldModel target;
    chunk.applyTo( target );

    const Engine::WorldTileModel* unknown = target.tile( 0, 0, 0 );
    EXPECT_TRUE( unknown == nullptr || unknown->tileType() == 0u );
    ASSERT_NE( target.tile( 1, 0, 0 ), nullptr );
    EXPECT_EQ( target.tile( 1, 0, 0 )->tileType(), GROUND_TILE );
}

TEST( ChunkDTOTest, Apply_Twice_IsIdempotent ) {
    const Engine::WorldModel source = makeWorld();
    const Engine::ChunkDTO chunk = Engine::ChunkDTO::fromModel( source, 1, 0 );

    Engine::WorldModel target;
    chunk.applyTo( target );
    chunk.applyTo( target );

    EXPECT_EQ( target.tile( CHUNK_SIZE + 2, 3, 0 )->tileType(), GROUND_TILE );
    EXPECT_EQ( target.object( CHUNK_SIZE + 5, 6, 0 )->objectType(), TREE_OBJECT );
}

TEST( ChunkDTOTest, Apply_AfterRemoveChunk_RestoresTerrain ) {
    const Engine::WorldModel source = makeWorld();
    const Engine::ChunkDTO chunk = Engine::ChunkDTO::fromModel( source, 1, 0 );

    Engine::WorldModel target;
    chunk.applyTo( target );
    target.removeChunk( 1, 0 );
    chunk.applyTo( target );

    ASSERT_NE( target.tile( CHUNK_SIZE + 2, 3, 0 ), nullptr );
    EXPECT_EQ( target.tile( CHUNK_SIZE + 2, 3, 0 )->tileType(), GROUND_TILE );
}

TEST( ChunkDTOTest, Apply_DoesNotTouchOtherChunks ) {
    const Engine::WorldModel source = makeWorld();

    Engine::WorldModel target;
    Engine::ChunkDTO::fromModel( source, 1, 0 ).applyTo( target );

    const Engine::WorldModel& loaded = target;

    EXPECT_EQ( loaded.chunk( 0, 0 ), nullptr );
    EXPECT_NE( loaded.chunk( 1, 0 ), nullptr );
}

TEST( ChunkDTOTest, FromJson_TilesWithWrongSize_AreIgnored ) {
    Json::Value floorJson;
    floorJson[ "z" ] = 0;
    floorJson[ "tiles" ] = Json::Value( Json::arrayValue );
    floorJson[ "tiles" ].append( GROUND_TILE );

    Json::Value json;
    json[ "floors" ] = Json::Value( Json::arrayValue );
    json[ "floors" ].append( floorJson );

    const Engine::ChunkDTO chunk = Engine::ChunkDTO::fromJson( json );

    ASSERT_EQ( chunk.floors().size(), 1u );
    for ( uint32_t tile : chunk.floors().front().tiles ) {
        EXPECT_EQ( tile, 0u );
    }
}

TEST( ChunkDTOTest, WorldModelRemoveChunk_MakesItsTilesUnavailable ) {
    Engine::WorldModel world = makeWorld();
    ASSERT_NE( world.tile( CHUNK_SIZE + 2, 3, 0 ), nullptr );

    world.removeChunk( 1, 0 );

    const Engine::WorldModel& remaining = world;

    EXPECT_EQ( remaining.tile( CHUNK_SIZE + 2, 3, 0 ), nullptr );
    EXPECT_EQ( remaining.chunk( 1, 0 ), nullptr );
}

TEST( ChunkEventLeaveDTOTest, ToJsonThenFromJson_RoundTripsCoordinates ) {
    Engine::ChunkEventLeaveDTO dto;
    dto.setChunkX( 3 );
    dto.setChunkY( -1 );

    const Json::Value json = dto.toJson();
    const Engine::ChunkEventLeaveDTO reloaded = Engine::ChunkEventLeaveDTO::fromJson( json );

    EXPECT_EQ( Engine::ServerMessageTypeHelper::fromMessage( json ), Engine::ServerMessageType::CHUNK_EVENT_LEAVE );
    EXPECT_EQ( reloaded.chunkX(), 3 );
    EXPECT_EQ( reloaded.chunkY(), -1 );
}
