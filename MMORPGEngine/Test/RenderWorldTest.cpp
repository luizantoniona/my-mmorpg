#include <gtest/gtest.h>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Tile/TileModel.h>
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

private:
    Engine::WorldModel& _world;
};

} // namespace

TEST( RenderWorldTest, ResolveFloor_ActiveFloorHasTile_ReturnsActiveFloor ) {
    Engine::TileModel tile;
    tile.setType( 9401 );
    tile.setName( "RenderWorldTestTile" );
    Engine::Singleton<Engine::DataManager>::instance().addTile( tile );

    Engine::WorldModel world;
    world.addFloor( 0 );
    world.setTile( 0, 0, 0, 9401 );

    TestRenderWorld renderWorld( world );

    EXPECT_EQ( renderWorld.resolveFloor( 0, 0, 0 ), 0 );
}

TEST( RenderWorldTest, ResolveFloor_ActiveFloorEmpty_FallsBackToNearestFloorBelowWithTile ) {
    Engine::TileModel tile;
    tile.setType( 9402 );
    tile.setName( "RenderWorldTestTile" );
    Engine::Singleton<Engine::DataManager>::instance().addTile( tile );

    Engine::WorldModel world;
    world.addFloor( -2 );
    world.addFloor( -1 );
    world.addFloor( 0 );
    world.setTile( 0, 0, -2, 9402 );

    TestRenderWorld renderWorld( world );

    EXPECT_EQ( renderWorld.resolveFloor( 0, 0, 0 ), -2 );
}

TEST( RenderWorldTest, ResolveFloor_NoFloorBelowHasTile_ReturnsActiveFloor ) {
    Engine::WorldModel world;
    world.addFloor( -1 );
    world.addFloor( 0 );

    TestRenderWorld renderWorld( world );

    EXPECT_EQ( renderWorld.resolveFloor( 0, 0, 0 ), 0 );
}
