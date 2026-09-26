#include <gtest/gtest.h>

#include <MMORPGEngine/Entity/EntityCombatModel.h>

TEST( EntityCombatModelTest, DefaultConstructed_IsNotReady ) {
    const Engine::EntityCombatModel combat;

    EXPECT_FALSE( combat.isReady( 20 ) );
}

TEST( EntityCombatModelTest, IsReady_CounterBelowCooldown_ReturnsFalse ) {
    Engine::EntityCombatModel combat;
    combat.setCooldownSeconds( 1.0 );
    combat.setCounter( 19 );

    EXPECT_FALSE( combat.isReady( 20 ) );
}

TEST( EntityCombatModelTest, IsReady_CounterEqualsCooldown_ReturnsTrue ) {
    Engine::EntityCombatModel combat;
    combat.setCooldownSeconds( 1.0 );
    combat.setCounter( 20 );

    EXPECT_TRUE( combat.isReady( 20 ) );
}

TEST( EntityCombatModelTest, IsReady_CounterAboveCooldown_ReturnsTrue ) {
    Engine::EntityCombatModel combat;
    combat.setCooldownSeconds( 1.0 );
    combat.setCounter( 25 );

    EXPECT_TRUE( combat.isReady( 20 ) );
}

TEST( EntityCombatModelTest, CooldownTicks_ConvertsSecondsUsingTickRate ) {
    Engine::EntityCombatModel combat;
    combat.setCooldownSeconds( 1.5 );

    EXPECT_EQ( combat.cooldownTicks( 20 ), 30 );
}
