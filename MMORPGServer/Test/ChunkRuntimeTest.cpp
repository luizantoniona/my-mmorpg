#include <gtest/gtest.h>

#include <MMORPGEngine/Data/Creature/CreatureSpawnAreaModel.h>
#include <MMORPGServer/Server/Runtime/Chunk/ChunkRuntime.h>
#include <MMORPGServer/Server/Runtime/World/Spawn/WorldSpawnAreaRuntime.h>

TEST( ChunkRuntimeTest, NewChunk_IsInactive ) {
    Server::ChunkRuntime chunk( Server::ChunkCoordinate( 0, 0, 0 ) );

    EXPECT_FALSE( chunk.hasObservers() );
    EXPECT_FALSE( chunk.hasPendingRespawn() );
    EXPECT_FALSE( chunk.isActive() );
}

TEST( ChunkRuntimeTest, Observer_ActivatesUntilLastOneLeaves ) {
    Server::ChunkRuntime chunk( Server::ChunkCoordinate( 0, 0, 0 ) );

    chunk.addObserver();
    chunk.addObserver();
    chunk.removeObserver();
    EXPECT_TRUE( chunk.isActive() );

    chunk.removeObserver();
    EXPECT_FALSE( chunk.isActive() );
}

TEST( ChunkRuntimeTest, RemoveObserver_WithoutObservers_StaysAtZero ) {
    Server::ChunkRuntime chunk( Server::ChunkCoordinate( 0, 0, 0 ) );

    chunk.removeObserver();
    chunk.addObserver();

    EXPECT_TRUE( chunk.hasObservers() );

    chunk.removeObserver();

    EXPECT_FALSE( chunk.hasObservers() );
}

TEST( ChunkRuntimeTest, PendingRespawn_ActivatesWithoutObservers ) {
    Server::WorldSpawnAreaRuntime area( Engine::CreatureSpawnAreaModel(), 0 );
    Server::ChunkRuntime chunk( Server::ChunkCoordinate( 0, 0, 0 ) );
    chunk.spawnAreas().push_back( &area );

    EXPECT_FALSE( chunk.isActive() );

    area.addPending( QStringLiteral( "3" ), 10 );

    EXPECT_TRUE( chunk.hasPendingRespawn() );
    EXPECT_TRUE( chunk.isActive() );
    EXPECT_FALSE( chunk.hasObservers() );

    area.pending().clear();

    EXPECT_FALSE( chunk.isActive() );
}
