#include <gtest/gtest.h>

#include <memory>

#include <MMORPGClient/Client/Renderer/ClientRenderWorld.h>

TEST( ClientRenderWorldTest, NoWorldSet_SizeIsZero ) {
    const ClientRenderWorld renderWorld;

    EXPECT_EQ( renderWorld.width(), 0u );
    EXPECT_EQ( renderWorld.height(), 0u );
    EXPECT_FALSE( renderWorld.hasTile( 0, 0, 0 ) );
    EXPECT_FALSE( renderWorld.hasObject( 0, 0, 0 ) );
}

TEST( ClientRenderWorldTest, SetWorld_UsesWorldDimensions ) {
    Engine::WorldModel world;
    world.setWidth( 64 );
    world.setHeight( 32 );

    ClientRenderWorld renderWorld;
    renderWorld.setWorld( &world );

    EXPECT_EQ( renderWorld.width(), 64u );
    EXPECT_EQ( renderWorld.height(), 32u );
}

TEST( ClientRenderWorldTest, SetWorld_EmitsBoundsChanged ) {
    Engine::WorldModel world;

    ClientRenderWorld renderWorld;
    int emitCount = 0;
    QObject::connect( &renderWorld, &Engine::RenderWorld::boundsChanged, [ &emitCount ]() {
        ++emitCount;
    } );

    renderWorld.setWorld( &world );

    EXPECT_EQ( emitCount, 1 );
}

TEST( ClientRenderWorldTest, TileAndObject_DelegateToWorld ) {
    Engine::WorldModel world;

    auto tile = std::make_unique<Engine::WorldTileModel>();
    tile->setTileType( 5 );
    world.chunk( 0, 0 )->setTile( 1, 1, 0, std::move( tile ) );

    ClientRenderWorld renderWorld;
    renderWorld.setWorld( &world );

    EXPECT_TRUE( renderWorld.hasTile( 1, 1, 0 ) );
    ASSERT_NE( renderWorld.tile( 1, 1, 0 ), nullptr );
    EXPECT_EQ( renderWorld.tile( 1, 1, 0 )->tileType(), 5u );
}

TEST( ClientRenderWorldTest, SetEntity_ThenEntities_ReturnsEntityAtMatchingFloor ) {
    ClientRenderWorld renderWorld;

    renderWorld.setEntity( 1, 10, 20, 0 );
    renderWorld.setEntity( 2, 30, 40, 1 );

    const QList<Engine::RenderWorld::Entity> floorZero = renderWorld.entities( 0 );

    ASSERT_EQ( floorZero.size(), 1 );
    EXPECT_EQ( floorZero.first().idEntity, 1 );
    EXPECT_EQ( floorZero.first().x, 10 );
    EXPECT_EQ( floorZero.first().y, 20 );
}

TEST( ClientRenderWorldTest, SetEntity_SameIdTwice_UpdatesInPlace ) {
    ClientRenderWorld renderWorld;

    renderWorld.setEntity( 1, 10, 20, 0 );
    renderWorld.setEntity( 1, 11, 21, 0 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_EQ( entities.first().x, 11 );
    EXPECT_EQ( entities.first().y, 21 );
}

TEST( ClientRenderWorldTest, RemoveEntity_RemovesFromEntities ) {
    ClientRenderWorld renderWorld;
    renderWorld.setEntity( 1, 10, 20, 0 );

    renderWorld.removeEntity( 1 );

    EXPECT_TRUE( renderWorld.entities( 0 ).isEmpty() );
}

TEST( ClientRenderWorldTest, ClearEntities_RemovesAllFloors ) {
    ClientRenderWorld renderWorld;
    renderWorld.setEntity( 1, 10, 20, 0 );
    renderWorld.setEntity( 2, 30, 40, 1 );

    renderWorld.clearEntities();

    EXPECT_TRUE( renderWorld.entities( 0 ).isEmpty() );
    EXPECT_TRUE( renderWorld.entities( 1 ).isEmpty() );
}
