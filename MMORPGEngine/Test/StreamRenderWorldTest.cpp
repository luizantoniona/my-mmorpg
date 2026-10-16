#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <string>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
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

TEST( StreamRenderWorldTest, AddCreature_WithFootprint_ExposesFootprintAndScaledTexture ) {
    TestStreamRenderWorld renderWorld;

    renderWorld.addCreature( 1, 30, 40, 0, 0.0, 3 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_EQ( entities.first().footprint, 3 );
    EXPECT_EQ( entities.first().texture.width(), Engine::WorldConstants::TILE_SIZE * 3 );
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

namespace {

class MovementOffsetCase {
public:
    MovementOffsetCase( std::string name, int toX, int toY, int toZ, double movementSeconds, int expectedSignX, int expectedSignY ) :
        name( std::move( name ) ),
        toX( toX ),
        toY( toY ),
        toZ( toZ ),
        movementSeconds( movementSeconds ),
        expectedSignX( expectedSignX ),
        expectedSignY( expectedSignY ) {
    }

    std::string name;
    int toX;
    int toY;
    int toZ;
    double movementSeconds;
    int expectedSignX;
    int expectedSignY;
};

class StreamRenderWorldMovementOffsetTest : public ::testing::TestWithParam<MovementOffsetCase> {};

int signOf( double value ) {
    if ( value < 0.0 ) {
        return -1;
    }

    return value > 0.0 ? 1 : 0;
}

} // namespace

TEST_P( StreamRenderWorldMovementOffsetTest, AddCharacter_MovingFromTenTwenty_OffsetPointsBackToPreviousTile ) {
    TestStreamRenderWorld renderWorld;
    renderWorld.addCharacter( 1, 10, 20, 0, 1.0 );

    renderWorld.addCharacter( 1, GetParam().toX, GetParam().toY, GetParam().toZ, GetParam().movementSeconds );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( GetParam().toZ );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_EQ( entities.first().x, GetParam().toX );
    EXPECT_EQ( signOf( entities.first().offsetX ), GetParam().expectedSignX );
    EXPECT_EQ( signOf( entities.first().offsetY ), GetParam().expectedSignY );
    EXPECT_LE( std::abs( entities.first().offsetX ), static_cast<double>( Engine::WorldConstants::TILE_SIZE ) );
    EXPECT_LE( std::abs( entities.first().offsetY ), static_cast<double>( Engine::WorldConstants::TILE_SIZE ) );
}

INSTANTIATE_TEST_SUITE_P(
    Steps,
    StreamRenderWorldMovementOffsetTest,
    ::testing::Values(
        MovementOffsetCase( "StepEast_OffsetPointsWest", 11, 20, 0, 1.0, -1, 0 ),
        MovementOffsetCase( "StepWest_OffsetPointsEast", 9, 20, 0, 1.0, 1, 0 ),
        MovementOffsetCase( "StepNorth_OffsetPointsSouth", 10, 19, 0, 1.0, 0, 1 ),
        MovementOffsetCase( "StepSouth_OffsetPointsNorth", 10, 21, 0, 1.0, 0, -1 ),
        MovementOffsetCase( "FarAway_SnapsWithoutOffset", 40, 20, 0, 1.0, 0, 0 ),
        MovementOffsetCase( "ChangesFloor_SnapsWithoutOffset", 11, 20, 1, 1.0, 0, 0 ),
        MovementOffsetCase( "WithoutMovementSeconds_SnapsWithoutOffset", 11, 20, 0, 0.0, 0, 0 ) ),
    []( const ::testing::TestParamInfo<MovementOffsetCase>& info ) { return info.param.name; } );

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

TEST( StreamRenderWorldTest, AddGroundItem_AppearsInEntitiesOfItsFloorOnly ) {
    TestStreamRenderWorld renderWorld;

    renderWorld.addGroundItem( 3, "IRON_SWORD", 10, 20, 0 );
    renderWorld.addGroundItem( 4, "IRON_SWORD", 11, 21, 1 );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_EQ( entities.first().x, 10 );
    EXPECT_EQ( entities.first().y, 20 );
}

TEST( StreamRenderWorldTest, AddGroundItem_DoesNotCollideWithCharacterEntityId ) {
    TestStreamRenderWorld renderWorld;

    renderWorld.addCharacter( 3, 10, 20, 0 );
    renderWorld.addGroundItem( 3, "IRON_SWORD", 11, 20, 0 );

    int matchingCharacterId = 0;
    for ( const Engine::RenderWorld::Entity& entity : renderWorld.entities( 0 ) ) {
        matchingCharacterId += entity.idEntity == 3 ? 1 : 0;
    }

    EXPECT_EQ( matchingCharacterId, 1 );
}

TEST( StreamRenderWorldTest, GroundItemAt_ReturnsTopmostIdAtTile ) {
    TestStreamRenderWorld renderWorld;
    renderWorld.addGroundItem( 3, "IRON_SWORD", 10, 20, 0 );
    renderWorld.addGroundItem( 5, "IRON_SWORD", 10, 20, 0 );

    EXPECT_EQ( renderWorld.groundItemAt( 10, 20, 0 ), 5 );
    EXPECT_EQ( renderWorld.groundItemAt( 10, 20, 1 ), 0 );
    EXPECT_EQ( renderWorld.groundItemAt( 11, 20, 0 ), 0 );
}

TEST( StreamRenderWorldTest, RemoveGroundItem_RemovesFromEntitiesAndLookup ) {
    TestStreamRenderWorld renderWorld;
    renderWorld.addGroundItem( 3, "IRON_SWORD", 10, 20, 0 );

    renderWorld.removeGroundItem( 3 );

    EXPECT_TRUE( renderWorld.entities( 0 ).isEmpty() );
    EXPECT_EQ( renderWorld.groundItemAt( 10, 20, 0 ), 0 );
}

TEST( StreamRenderWorldTest, ClearEntities_RemovesGroundItems ) {
    TestStreamRenderWorld renderWorld;
    renderWorld.addGroundItem( 3, "IRON_SWORD", 10, 20, 0 );

    renderWorld.clearEntities();

    EXPECT_EQ( renderWorld.groundItemAt( 10, 20, 0 ), 0 );
}

namespace {

constexpr int IDLE_SOUTH_WIDTH = 5;
constexpr int IDLE_NORTH_WIDTH = 6;
constexpr int WALK_WEST_WIDTH = 7;
constexpr int ATTACK_EAST_WIDTH = 8;

Engine::AnimationModel singleFrame( int width ) {
    QImage image( width, 4, QImage::Format_ARGB32_Premultiplied );
    image.fill( Qt::red );

    return Engine::AnimationModel( { image }, 100 );
}

void registerSpriteCreatureType() {
    Engine::AppearanceModel appearance;
    appearance.setAnimation( Engine::AppearanceStateEnum::IDLE, Engine::AppearanceDirectionEnum::SOUTH, singleFrame( IDLE_SOUTH_WIDTH ) );
    appearance.setAnimation( Engine::AppearanceStateEnum::IDLE, Engine::AppearanceDirectionEnum::NORTH, singleFrame( IDLE_NORTH_WIDTH ) );
    appearance.setAnimation( Engine::AppearanceStateEnum::WALK, Engine::AppearanceDirectionEnum::WEST, singleFrame( WALK_WEST_WIDTH ) );
    appearance.setAnimation( Engine::AppearanceStateEnum::ATTACK, Engine::AppearanceDirectionEnum::EAST, singleFrame( ATTACK_EAST_WIDTH ) );

    Engine::CreatureTypeModel creatureType;
    creatureType.setType( "STREAM_TEST_SPRITE" );
    creatureType.setAppearance( appearance );

    Engine::Singleton<Engine::DataManager>::instance().addCreatureType( creatureType );
}

} // namespace

TEST( StreamRenderWorldTest, AddCreature_WithAppearance_UsesIdleFrameFacingSouth ) {
    registerSpriteCreatureType();
    TestStreamRenderWorld renderWorld;

    renderWorld.addCreature( 1, 10, 20, 0, 0.0, 1, "STREAM_TEST_SPRITE" );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_EQ( entities.first().texture.width(), IDLE_SOUTH_WIDTH );
}

TEST( StreamRenderWorldTest, AddCreature_UnknownCreatureType_FallsBackToPlaceholder ) {
    TestStreamRenderWorld renderWorld;

    renderWorld.addCreature( 1, 10, 20, 0, 0.0, 1, "NOT_REGISTERED" );

    const QList<Engine::RenderWorld::Entity> entities = renderWorld.entities( 0 );

    ASSERT_EQ( entities.size(), 1 );
    EXPECT_EQ( entities.first().texture.cacheKey(), Engine::EntityTextureModel::creatureTexture().cacheKey() );
}

TEST( StreamRenderWorldTest, AddCreature_MovedNorth_FacesNorthWhenIdle ) {
    registerSpriteCreatureType();
    TestStreamRenderWorld renderWorld;
    renderWorld.addCreature( 1, 10, 20, 0, 0.0, 1, "STREAM_TEST_SPRITE" );

    renderWorld.addCreature( 1, 10, 19, 0, 0.0, 1, "STREAM_TEST_SPRITE" );

    ASSERT_EQ( renderWorld.entities( 0 ).size(), 1 );
    EXPECT_EQ( renderWorld.entities( 0 ).first().texture.width(), IDLE_NORTH_WIDTH );
}

TEST( StreamRenderWorldTest, AddCreature_WhileMovingWest_UsesWalkClip ) {
    registerSpriteCreatureType();
    TestStreamRenderWorld renderWorld;
    renderWorld.addCreature( 1, 10, 20, 0, 60.0, 1, "STREAM_TEST_SPRITE" );

    renderWorld.addCreature( 1, 9, 20, 0, 60.0, 1, "STREAM_TEST_SPRITE" );

    ASSERT_EQ( renderWorld.entities( 0 ).size(), 1 );
    EXPECT_EQ( renderWorld.entities( 0 ).first().texture.width(), WALK_WEST_WIDTH );
}

TEST( StreamRenderWorldTest, StartCreatureAttack_FacesTargetAndUsesAttackClip ) {
    registerSpriteCreatureType();
    TestStreamRenderWorld renderWorld;
    renderWorld.addCreature( 1, 10, 20, 0, 0.0, 1, "STREAM_TEST_SPRITE" );

    renderWorld.startCreatureAttack( 1, 11, 20, 60.0 );

    ASSERT_EQ( renderWorld.entities( 0 ).size(), 1 );
    EXPECT_EQ( renderWorld.entities( 0 ).first().texture.width(), ATTACK_EAST_WIDTH );
}

TEST( StreamRenderWorldTest, StartCreatureAttack_UnknownCreature_IsIgnored ) {
    TestStreamRenderWorld renderWorld;

    renderWorld.startCreatureAttack( 99, 11, 20, 1.0 );

    EXPECT_TRUE( renderWorld.entities( 0 ).isEmpty() );
}
