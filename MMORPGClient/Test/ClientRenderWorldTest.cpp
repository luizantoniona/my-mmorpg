#include <gtest/gtest.h>

#include <algorithm>
#include <memory>

#include <MMORPGClient/Client/Renderer/ClientRenderWorld.h>
#include <MMORPGEngine/Renderer/EntityTextureModel.h>

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

TEST( ClientRenderWorldTest, AddCharacter_ThenEntities_ReturnsEntityAtMatchingFloor ) {
    ClientRenderWorld renderWorld;

    renderWorld.addCharacter( 1, 10, 20, 0 );
    renderWorld.addCharacter( 2, 30, 40, 1 );

    const QList<Engine::RenderWorld::Entity> floorZero = renderWorld.entities( 0 );

    ASSERT_EQ( floorZero.size(), 1 );
    EXPECT_EQ( floorZero.first().idEntity, 1 );
    EXPECT_EQ( floorZero.first().x, 10 );
    EXPECT_EQ( floorZero.first().y, 20 );
    EXPECT_EQ( floorZero.first().texture.cacheKey(), Engine::EntityTextureModel::characterTexture().cacheKey() );
}

TEST( ClientRenderWorldTest, AddCharacter_SameIdTwice_UpdatesInPlace ) {
    ClientRenderWorld renderWorld;

    renderWorld.addCharacter( 1, 10, 20, 0 );
    renderWorld.addCharacter( 1, 11, 21, 0 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_EQ( entities.first().x, 11 );
    EXPECT_EQ( entities.first().y, 21 );
}

TEST( ClientRenderWorldTest, RemoveCharacter_RemovesFromEntities ) {
    ClientRenderWorld renderWorld;
    renderWorld.addCharacter( 1, 10, 20, 0 );

    renderWorld.removeCharacter( 1 );

    EXPECT_TRUE( renderWorld.entities( 0 ).isEmpty() );
}

TEST( ClientRenderWorldTest, ClearEntities_RemovesAllFloors ) {
    ClientRenderWorld renderWorld;
    renderWorld.addCharacter( 1, 10, 20, 0 );
    renderWorld.addCharacter( 2, 30, 40, 1 );

    renderWorld.clearEntities();

    EXPECT_TRUE( renderWorld.entities( 0 ).isEmpty() );
    EXPECT_TRUE( renderWorld.entities( 1 ).isEmpty() );
}

TEST( ClientRenderWorldTest, AddCharacterAndCreature_SameId_AreIndependent ) {
    ClientRenderWorld renderWorld;

    renderWorld.addCharacter( 1, 10, 20, 0 );
    renderWorld.addCreature( 1, 30, 40, 0 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 2 );

    const bool hasCharacter = std::any_of( entities.begin(), entities.end(), []( const Engine::RenderWorld::Entity& entity ) {
        return entity.x == 10 && entity.y == 20 && entity.texture.cacheKey() == Engine::EntityTextureModel::characterTexture().cacheKey();
    } );
    const bool hasCreature = std::any_of( entities.begin(), entities.end(), []( const Engine::RenderWorld::Entity& entity ) {
        return entity.x == 30 && entity.y == 40 && entity.texture.cacheKey() == Engine::EntityTextureModel::creatureTexture().cacheKey();
    } );

    EXPECT_TRUE( hasCharacter );
    EXPECT_TRUE( hasCreature );
}

TEST( ClientRenderWorldTest, RemoveCreature_DoesNotAffectCharacterWithSameId ) {
    ClientRenderWorld renderWorld;

    renderWorld.addCharacter( 1, 10, 20, 0 );
    renderWorld.addCreature( 1, 30, 40, 0 );

    renderWorld.removeCreature( 1 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_EQ( entities.first().texture.cacheKey(), Engine::EntityTextureModel::characterTexture().cacheKey() );
}

TEST( ClientRenderWorldTest, SetOwnCharacter_AppearsInEntities ) {
    ClientRenderWorld renderWorld;

    renderWorld.setOwnCharacter( 1, 10, 20, 0 );
    renderWorld.addCharacter( 2, 30, 40, 0 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 2 );

    const bool hasOwnCharacter = std::any_of( entities.begin(), entities.end(), []( const Engine::RenderWorld::Entity& entity ) {
        return entity.idEntity == 1 && entity.x == 10 && entity.y == 20;
    } );

    EXPECT_TRUE( hasOwnCharacter );
}

TEST( ClientRenderWorldTest, SetOwnCharacter_Twice_KeepsOnlyLatest ) {
    ClientRenderWorld renderWorld;

    renderWorld.setOwnCharacter( 1, 10, 20, 0 );
    renderWorld.setOwnCharacter( 2, 30, 40, 0 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_EQ( entities.first().idEntity, 2 );
}

TEST( ClientRenderWorldTest, ClearEntities_RemovesOwnCharacter ) {
    ClientRenderWorld renderWorld;
    renderWorld.setOwnCharacter( 1, 10, 20, 0 );

    renderWorld.clearEntities();

    EXPECT_TRUE( renderWorld.entities( 0 ).isEmpty() );
}
