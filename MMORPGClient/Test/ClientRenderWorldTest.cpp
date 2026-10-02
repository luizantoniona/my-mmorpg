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
