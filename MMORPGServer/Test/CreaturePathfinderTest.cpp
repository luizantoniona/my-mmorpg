#include <gtest/gtest.h>

#include <MMORPGServer/Server/Runtime/Creature/CreaturePathfinder.h>

namespace {

Engine::EntityPositionModel positionAt( int x, int y ) {
    Engine::EntityPositionModel position;
    position.setX( x );
    position.setY( y );
    position.setZ( 0 );
    return position;
}

bool alwaysWalkable( int, int, int ) {
    return true;
}

bool neverOccupied( int, int, int ) {
    return false;
}

} // namespace

TEST( CreaturePathfinderTest, NextStep_OpenField_MovesTowardGoal ) {
    const auto step = Server::CreaturePathfinder::nextStep( positionAt( 0, 0 ), positionAt( 5, 0 ), 1, alwaysWalkable, neverOccupied );

    ASSERT_TRUE( step.has_value() );
    EXPECT_EQ( step->x(), 1 );
    EXPECT_EQ( step->y(), 0 );
}

TEST( CreaturePathfinderTest, NextStep_AlreadyWithinReach_ReturnsNothing ) {
    const auto step = Server::CreaturePathfinder::nextStep( positionAt( 0, 0 ), positionAt( 1, 1 ), 1, alwaysWalkable, neverOccupied );

    EXPECT_FALSE( step.has_value() );
}

TEST( CreaturePathfinderTest, NextStep_RangedReach_StopsAtRangeInsteadOfAdjacent ) {
    EXPECT_FALSE( Server::CreaturePathfinder::nextStep( positionAt( 0, 0 ), positionAt( 3, 0 ), 3, alwaysWalkable, neverOccupied ).has_value() );

    const auto step = Server::CreaturePathfinder::nextStep( positionAt( 0, 0 ), positionAt( 5, 0 ), 3, alwaysWalkable, neverOccupied );

    ASSERT_TRUE( step.has_value() );
    EXPECT_EQ( step->x(), 1 );
}

TEST( CreaturePathfinderTest, NextStep_WallInTheWay_GoesAround ) {
    const auto isWalkable = []( int x, int y, int ) { return !( x == 1 && y >= -1 && y <= 1 ); };

    const auto step = Server::CreaturePathfinder::nextStep( positionAt( 0, 0 ), positionAt( 3, 0 ), 0, isWalkable, neverOccupied );

    ASSERT_TRUE( step.has_value() );
    EXPECT_NE( step->x(), 1 );
}

TEST( CreaturePathfinderTest, NextStep_GoalWalledOff_ReturnsNothing ) {
    const auto isWalkable = []( int x, int, int ) { return x < 2; };

    const auto step = Server::CreaturePathfinder::nextStep( positionAt( 0, 0 ), positionAt( 6, 0 ), 1, isWalkable, neverOccupied );

    EXPECT_FALSE( step.has_value() );
}

TEST( CreaturePathfinderTest, NextStep_OccupiedTile_IsAvoided ) {
    const auto isOccupied = []( int x, int y, int ) { return x == 1 && y == 0; };

    const auto step = Server::CreaturePathfinder::nextStep( positionAt( 0, 0 ), positionAt( 5, 0 ), 1, alwaysWalkable, isOccupied );

    ASSERT_TRUE( step.has_value() );
    EXPECT_FALSE( step->x() == 1 && step->y() == 0 );
}

TEST( CreaturePathfinderTest, NextStep_DifferentFloor_ReturnsNothing ) {
    Engine::EntityPositionModel goal = positionAt( 3, 0 );
    goal.setZ( 1 );

    const auto step = Server::CreaturePathfinder::nextStep( positionAt( 0, 0 ), goal, 1, alwaysWalkable, neverOccupied );

    EXPECT_FALSE( step.has_value() );
}
