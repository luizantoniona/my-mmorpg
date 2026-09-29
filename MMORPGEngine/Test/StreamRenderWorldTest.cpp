#include <gtest/gtest.h>

#include <algorithm>

#include <MMORPGEngine/Renderer/EntityTextureModel.h>
#include <MMORPGEngine/Renderer/World/StreamRenderWorld.h>
#include <MMORPGEngine/World/WorldConstants.h>

namespace {

class TestStreamRenderWorld : public Engine::StreamRenderWorld {
public:
    TestStreamRenderWorld() = default;

    const Engine::WorldObjectModel* object( int, int, int ) const override {
        return nullptr;
    }

    const Engine::WorldTileModel* tile( int, int, int ) const override {
        return nullptr;
    }

    std::vector<int> floors() const override {
        return {};
    }

    uint32_t width() const override {
        return 0;
    }

    uint32_t height() const override {
        return 0;
    }
};

} // namespace

TEST( StreamRenderWorldTest, AddCharacter_ThenEntities_ReturnsEntityAtMatchingFloor ) {
    TestStreamRenderWorld renderWorld;

    renderWorld.addCharacter( 1, 10, 20, 0 );
    renderWorld.addCharacter( 2, 30, 40, 1 );

    const QList<Engine::RenderWorld::Entity> floorZero = renderWorld.entities( 0 );

    ASSERT_EQ( floorZero.size(), 1 );
    EXPECT_EQ( floorZero.first().idEntity, 1 );
    EXPECT_EQ( floorZero.first().x, 10 );
    EXPECT_EQ( floorZero.first().y, 20 );
    EXPECT_EQ( floorZero.first().texture.cacheKey(), Engine::EntityTextureModel::characterTexture().cacheKey() );
}

TEST( StreamRenderWorldTest, AddCharacter_SameIdTwice_UpdatesInPlace ) {
    TestStreamRenderWorld renderWorld;

    renderWorld.addCharacter( 1, 10, 20, 0 );
    renderWorld.addCharacter( 1, 11, 21, 0 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_EQ( entities.first().x, 11 );
    EXPECT_EQ( entities.first().y, 21 );
}

TEST( StreamRenderWorldTest, RemoveCharacter_RemovesFromEntities ) {
    TestStreamRenderWorld renderWorld;
    renderWorld.addCharacter( 1, 10, 20, 0 );

    renderWorld.removeCharacter( 1 );

    EXPECT_TRUE( renderWorld.entities( 0 ).isEmpty() );
}

TEST( StreamRenderWorldTest, ClearEntities_RemovesAllFloors ) {
    TestStreamRenderWorld renderWorld;
    renderWorld.addCharacter( 1, 10, 20, 0 );
    renderWorld.addCharacter( 2, 30, 40, 1 );

    renderWorld.clearEntities();

    EXPECT_TRUE( renderWorld.entities( 0 ).isEmpty() );
    EXPECT_TRUE( renderWorld.entities( 1 ).isEmpty() );
}

TEST( StreamRenderWorldTest, AddCharacterAndCreature_SameId_AreIndependent ) {
    TestStreamRenderWorld renderWorld;

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

TEST( StreamRenderWorldTest, RemoveCreature_DoesNotAffectCharacterWithSameId ) {
    TestStreamRenderWorld renderWorld;

    renderWorld.addCharacter( 1, 10, 20, 0 );
    renderWorld.addCreature( 1, 30, 40, 0 );

    renderWorld.removeCreature( 1 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_EQ( entities.first().texture.cacheKey(), Engine::EntityTextureModel::characterTexture().cacheKey() );
}

TEST( StreamRenderWorldTest, SetOwnCharacter_AppearsInEntities ) {
    TestStreamRenderWorld renderWorld;

    renderWorld.setOwnCharacter( 1, 10, 20, 0 );
    renderWorld.addCharacter( 2, 30, 40, 0 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 2 );

    const bool hasOwnCharacter = std::any_of( entities.begin(), entities.end(), []( const Engine::RenderWorld::Entity& entity ) {
        return entity.idEntity == 1 && entity.x == 10 && entity.y == 20;
    } );

    EXPECT_TRUE( hasOwnCharacter );
}

TEST( StreamRenderWorldTest, SetOwnCharacter_Twice_KeepsOnlyLatest ) {
    TestStreamRenderWorld renderWorld;

    renderWorld.setOwnCharacter( 1, 10, 20, 0 );
    renderWorld.setOwnCharacter( 2, 30, 40, 0 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_EQ( entities.first().idEntity, 2 );
}

TEST( StreamRenderWorldTest, ClearEntities_RemovesOwnCharacter ) {
    TestStreamRenderWorld renderWorld;
    renderWorld.setOwnCharacter( 1, 10, 20, 0 );

    renderWorld.clearEntities();

    EXPECT_TRUE( renderWorld.entities( 0 ).isEmpty() );
}

TEST( StreamRenderWorldTest, AddCharacter_FirstTime_HasNoAnimationOffset ) {
    TestStreamRenderWorld renderWorld;

    renderWorld.addCharacter( 1, 10, 20, 0, 1.0 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_DOUBLE_EQ( entities.first().offsetX, 0.0 );
    EXPECT_DOUBLE_EQ( entities.first().offsetY, 0.0 );
}

TEST( StreamRenderWorldTest, AddCharacter_MovesToNeighbourTile_OffsetPointsBackToPreviousTile ) {
    TestStreamRenderWorld renderWorld;
    renderWorld.addCharacter( 1, 10, 20, 0, 1.0 );

    renderWorld.addCharacter( 1, 11, 20, 0, 1.0 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_EQ( entities.first().x, 11 );
    EXPECT_LT( entities.first().offsetX, 0.0 );
    EXPECT_GE( entities.first().offsetX, -1.0 * Engine::WorldConstants::TILE_SIZE );
    EXPECT_DOUBLE_EQ( entities.first().offsetY, 0.0 );
}

TEST( StreamRenderWorldTest, AddCharacter_MovesFarAway_SnapsWithoutOffset ) {
    TestStreamRenderWorld renderWorld;
    renderWorld.addCharacter( 1, 10, 20, 0, 1.0 );

    renderWorld.addCharacter( 1, 40, 20, 0, 1.0 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_DOUBLE_EQ( entities.first().offsetX, 0.0 );
}

TEST( StreamRenderWorldTest, AddCharacter_ChangesFloor_SnapsWithoutOffset ) {
    TestStreamRenderWorld renderWorld;
    renderWorld.addCharacter( 1, 10, 20, 0, 1.0 );

    renderWorld.addCharacter( 1, 11, 20, 1, 1.0 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 1 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_DOUBLE_EQ( entities.first().offsetX, 0.0 );
}

TEST( StreamRenderWorldTest, AddCharacter_WithoutMovementSeconds_SnapsWithoutOffset ) {
    TestStreamRenderWorld renderWorld;
    renderWorld.addCharacter( 1, 10, 20, 0 );

    renderWorld.addCharacter( 1, 11, 20, 0 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_DOUBLE_EQ( entities.first().offsetX, 0.0 );
}

TEST( StreamRenderWorldTest, SetOwnCharacter_MovesToNeighbourTile_KeepsAnimationState ) {
    TestStreamRenderWorld renderWorld;
    renderWorld.setOwnCharacter( 1, 10, 20, 0, 1.0 );

    renderWorld.setOwnCharacter( 1, 10, 21, 0, 1.0 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_EQ( entities.first().y, 21 );
    EXPECT_LT( entities.first().offsetY, 0.0 );
}

TEST( StreamRenderWorldTest, SetOwnCharacter_SameTileDuringMovement_KeepsOffset ) {
    TestStreamRenderWorld renderWorld;
    renderWorld.setOwnCharacter( 1, 10, 20, 0, 1.0 );
    renderWorld.setOwnCharacter( 1, 11, 20, 0, 1.0 );

    renderWorld.setOwnCharacter( 1, 11, 20, 0, 1.0 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_LT( entities.first().offsetX, 0.0 );
}

TEST( StreamRenderWorldTest, AddCreature_MovesToNeighbourTile_OffsetPointsBackToPreviousTile ) {
    TestStreamRenderWorld renderWorld;
    renderWorld.addCreature( 1, 10, 20, 0, 1.0 );

    renderWorld.addCreature( 1, 10, 19, 0, 1.0 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_EQ( entities.first().y, 19 );
    EXPECT_GT( entities.first().offsetY, 0.0 );
}
