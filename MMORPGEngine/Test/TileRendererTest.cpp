#include <gtest/gtest.h>

#include <QColor>
#include <QImage>
#include <QList>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Animation/AnimationModel.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Tile/TileModel.h>
#include <MMORPGEngine/Renderer/Camera/Camera.h>
#include <MMORPGEngine/Renderer/Scene/RenderScene.h>
#include <MMORPGEngine/Renderer/TileRenderer.h>
#include <MMORPGEngine/Renderer/World/RenderWorld.h>
#include <MMORPGEngine/World/WorldModel.h>

namespace {

class TestRenderWorld : public Engine::RenderWorld {
public:
    explicit TestRenderWorld( Engine::WorldModel& world ) :
        _world( world ) {
    }

    const Engine::WorldObjectModel* object( int x, int y, int z ) const override {
        return _world.object( x, y, z );
    }

    const Engine::WorldTileModel* tile( int x, int y, int z ) const override {
        return _world.tile( x, y, z );
    }

    QList<Engine::RenderWorld::Entity> entities( int ) const override {
        return {};
    }

    std::vector<int> floors() const override {
        return _world.floors();
    }

    uint32_t width() const override {
        return _world.width();
    }

    uint32_t height() const override {
        return _world.height();
    }

    bool hasChunk( int chunkX, int chunkY ) const override {
        return _world.hasChunk( chunkX, chunkY );
    }

    quint64 chunkRevision( int chunkX, int chunkY ) const override {
        return _world.chunkRevision( chunkX, chunkY );
    }

private:
    Engine::WorldModel& _world;
};

QImage solidFrame( const QColor& color ) {
    QImage frame( 32, 32, QImage::Format_ARGB32 );
    frame.fill( color );

    return frame;
}

void registerStaticTile( uint32_t type ) {
    Engine::TileModel tile;
    tile.setType( type );
    tile.setName( "TileRendererTestTile" );
    tile.setTexture( solidFrame( Qt::green ) );

    Engine::Singleton<Engine::DataManager>::instance().addTile( tile );
}

void registerAnimatedTile( uint32_t type ) {
    Engine::TileModel tile;
    tile.setType( type );
    tile.setName( "TileRendererTestAnimatedTile" );
    tile.setAnimation( Engine::AnimationModel( { solidFrame( Qt::blue ), solidFrame( Qt::cyan ) }, 100 ) );

    Engine::Singleton<Engine::DataManager>::instance().addTile( tile );
}

Engine::WorldModel makeWorld() {
    Engine::WorldModel world;
    world.setWidth( 64 );
    world.setHeight( 64 );
    world.addFloor( 0 );

    return world;
}

Engine::Camera makeCamera( double zoom = 1.0 ) {
    Engine::Camera camera;
    camera.setViewportSize( QSizeF( 1000.0, 800.0 ) );
    camera.setZoom( zoom );
    camera.centerOnTile( 32, 16 );

    return camera;
}

int renderedItems( Engine::TileRenderer& renderer, const Engine::RenderWorld& world, const Engine::Camera& camera ) {
    Engine::RenderScene scene;
    renderer.render( scene, camera, world, 0 );

    return scene.itemCount();
}

} // namespace

TEST( TileRendererTest, Render_ManyStaticTilesInOneChunk_ProduceSingleItem ) {
    registerStaticTile( 9601 );

    Engine::WorldModel world = makeWorld();
    for ( int x = 0; x < 10; ++x ) {
        for ( int y = 0; y < 10; ++y ) {
            world.setTile( x + 20, y + 10, 0, 9601 );
        }
    }

    TestRenderWorld renderWorld( world );
    Engine::TileRenderer renderer;

    EXPECT_EQ( renderedItems( renderer, renderWorld, makeCamera() ), 1 );
}

TEST( TileRendererTest, Render_TilesInTwoChunks_ProduceOneItemPerChunk ) {
    registerStaticTile( 9602 );

    Engine::WorldModel world = makeWorld();
    world.setTile( 20, 10, 0, 9602 );
    world.setTile( 40, 10, 0, 9602 );

    TestRenderWorld renderWorld( world );
    Engine::TileRenderer renderer;

    EXPECT_EQ( renderedItems( renderer, renderWorld, makeCamera() ), 2 );
}

TEST( TileRendererTest, Render_ChunkNotLoaded_ProducesNothing ) {
    Engine::WorldModel world = makeWorld();

    TestRenderWorld renderWorld( world );
    Engine::TileRenderer renderer;

    EXPECT_EQ( renderedItems( renderer, renderWorld, makeCamera() ), 0 );
}

TEST( TileRendererTest, Render_ChunkOutsideVisibleArea_IsSkipped ) {
    registerStaticTile( 9603 );

    Engine::WorldModel world = makeWorld();
    world.setTile( 60, 60, 0, 9603 );

    TestRenderWorld renderWorld( world );
    Engine::TileRenderer renderer;

    Engine::Camera camera;
    camera.setViewportSize( QSizeF( 320.0, 320.0 ) );
    camera.centerOnTile( 5, 5 );

    EXPECT_EQ( renderedItems( renderer, renderWorld, camera ), 0 );
}

TEST( TileRendererTest, Render_AnimatedTile_IsDrawnPerFrameBesideTheBakedChunk ) {
    registerStaticTile( 9604 );
    registerAnimatedTile( 9605 );

    Engine::WorldModel world = makeWorld();
    world.setTile( 20, 10, 0, 9604 );
    world.setTile( 21, 10, 0, 9605 );

    TestRenderWorld renderWorld( world );
    Engine::TileRenderer renderer;

    EXPECT_EQ( renderedItems( renderer, renderWorld, makeCamera() ), 2 );
}

TEST( TileRendererTest, Render_AnimatedTileOnly_HasNoBakedItem ) {
    registerAnimatedTile( 9606 );

    Engine::WorldModel world = makeWorld();
    world.setTile( 20, 10, 0, 9606 );

    TestRenderWorld renderWorld( world );
    Engine::TileRenderer renderer;

    EXPECT_EQ( renderedItems( renderer, renderWorld, makeCamera() ), 1 );
}

TEST( TileRendererTest, Render_ZoomedOut_BakesAnimatedTileAsStatic ) {
    registerAnimatedTile( 9607 );

    Engine::WorldModel world = makeWorld();
    world.setTile( 20, 10, 0, 9607 );
    world.setTile( 21, 10, 0, 9607 );

    TestRenderWorld renderWorld( world );
    Engine::TileRenderer renderer;

    EXPECT_EQ( renderedItems( renderer, renderWorld, makeCamera( 0.25 ) ), 1 );
}

TEST( TileRendererTest, Render_AfterTileRemoved_RebakesTheChunk ) {
    registerStaticTile( 9608 );

    Engine::WorldModel world = makeWorld();
    world.setTile( 20, 10, 0, 9608 );

    TestRenderWorld renderWorld( world );
    Engine::TileRenderer renderer;
    const Engine::Camera camera = makeCamera();

    ASSERT_EQ( renderedItems( renderer, renderWorld, camera ), 1 );

    world.setTile( 20, 10, 0, 0 );

    EXPECT_EQ( renderedItems( renderer, renderWorld, camera ), 0 );
}

TEST( TileRendererTest, Render_ChunksReadByTheRenderer_AreNotRebakedEveryFrame ) {
    registerStaticTile( 9611 );

    Engine::WorldModel world = makeWorld();
    world.setWidth( 192 );
    world.setHeight( 64 );
    for ( int chunkX = 0; chunkX < 6; ++chunkX ) {
        world.setTile( chunkX * 32 + 1, 1, 0, 9611 );
    }

    TestRenderWorld renderWorld( world );
    Engine::TileRenderer renderer;

    Engine::Camera camera;
    camera.setViewportSize( QSizeF( 6400.0, 800.0 ) );
    camera.centerOnTile( 96, 16 );

    quint64 revisions[ 6 ];
    for ( int chunkX = 0; chunkX < 6; ++chunkX ) {
        revisions[ chunkX ] = world.chunkRevision( chunkX, 0 );
    }

    for ( int frame = 0; frame < 4; ++frame ) {
        renderedItems( renderer, renderWorld, camera );
    }

    for ( int chunkX = 0; chunkX < 6; ++chunkX ) {
        EXPECT_EQ( world.chunkRevision( chunkX, 0 ), revisions[ chunkX ] );
    }
}

TEST( TileRendererTest, Render_MoreUnbakedChunksThanTheFrameBudget_CatchUpOnNextFrames ) {
    registerStaticTile( 9610 );

    Engine::WorldModel world = makeWorld();
    world.setWidth( 192 );
    world.setHeight( 64 );
    for ( int chunkX = 0; chunkX < 6; ++chunkX ) {
        world.setTile( chunkX * 32 + 1, 1, 0, 9610 );
    }

    TestRenderWorld renderWorld( world );
    Engine::TileRenderer renderer;

    Engine::Camera camera;
    camera.setViewportSize( QSizeF( 6400.0, 800.0 ) );
    camera.centerOnTile( 96, 16 );

    const int firstFrame = renderedItems( renderer, renderWorld, camera );
    const int secondFrame = renderedItems( renderer, renderWorld, camera );

    EXPECT_LT( firstFrame, 6 );
    EXPECT_EQ( secondFrame, 6 );
}
