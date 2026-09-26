#include <gtest/gtest.h>

#include <MMORPGEngine/Entity/EntityMovementModel.h>

TEST( EntityMovementModelTest, DefaultConstructed_IsNotReady ) {
    const Engine::EntityMovementModel movement;

    EXPECT_FALSE( movement.isReady( 20 ) );
}

TEST( EntityMovementModelTest, IsReady_CounterBelowCooldown_ReturnsFalse ) {
    Engine::EntityMovementModel movement;
    movement.setCooldownSeconds( 1.0 );
    movement.setCounter( 19 );

    EXPECT_FALSE( movement.isReady( 20 ) );
}

TEST( EntityMovementModelTest, IsReady_CounterEqualsCooldown_ReturnsTrue ) {
    Engine::EntityMovementModel movement;
    movement.setCooldownSeconds( 1.0 );
    movement.setCounter( 20 );

    EXPECT_TRUE( movement.isReady( 20 ) );
}

TEST( EntityMovementModelTest, IsReady_CounterAboveCooldown_ReturnsTrue ) {
    Engine::EntityMovementModel movement;
    movement.setCooldownSeconds( 1.0 );
    movement.setCounter( 25 );

    EXPECT_TRUE( movement.isReady( 20 ) );
}

TEST( EntityMovementModelTest, CooldownTicks_ConvertsSecondsUsingTickRate ) {
    Engine::EntityMovementModel movement;
    movement.setCooldownSeconds( 1.5 );

    EXPECT_EQ( movement.cooldownTicks( 20 ), 30 );
}

TEST( EntityMovementModelTest, IsReady_SameCooldownSeconds_DifferentTickRate_NeedsMoreTicks ) {
    Engine::EntityMovementModel movement;
    movement.setCooldownSeconds( 1.0 );
    movement.setCounter( 20 );

    EXPECT_TRUE( movement.isReady( 20 ) );
    EXPECT_FALSE( movement.isReady( 30 ) );
}
