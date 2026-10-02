#include <gtest/gtest.h>

#include <MMORPGEngine/Entity/EntityCombatModel.h>

TEST( EntityCombatModelTest, DefaultConstructed_IsNotReady ) {
    const Engine::EntityCombatModel combat;

    EXPECT_FALSE( combat.isReady( 20 ) );
}

TEST( EntityCombatModelTest, DefaultConstructed_CanAttackAndDodgeButNotBlock ) {
    const Engine::EntityCombatModel combat;

    EXPECT_TRUE( combat.isActionAvailable( Engine::CombatActionEnum::ATTACK ) );
    EXPECT_TRUE( combat.isActionAvailable( Engine::CombatActionEnum::DODGE ) );
    EXPECT_FALSE( combat.isActionAvailable( Engine::CombatActionEnum::BLOCK ) );
}

TEST( EntityCombatModelTest, SetActionAvailable_TogglesAction ) {
    Engine::EntityCombatModel combat;

    combat.setActionAvailable( Engine::CombatActionEnum::BLOCK, true );
    EXPECT_TRUE( combat.isActionAvailable( Engine::CombatActionEnum::BLOCK ) );

    combat.setActionAvailable( Engine::CombatActionEnum::BLOCK, false );
    EXPECT_FALSE( combat.isActionAvailable( Engine::CombatActionEnum::BLOCK ) );
}

TEST( EntityCombatModelTest, SetAvailableActions_ReplacesWholeSet ) {
    Engine::EntityCombatModel combat;
    combat.setAvailableActions( { Engine::CombatActionEnum::BLOCK } );

    EXPECT_EQ( combat.availableActions().size(), 1u );
    EXPECT_FALSE( combat.isActionAvailable( Engine::CombatActionEnum::ATTACK ) );
}

TEST( EntityCombatModelTest, DefaultConstructed_AttackRangeIsMelee ) {
    const Engine::EntityCombatModel combat;

    EXPECT_EQ( combat.attackRange(), 1 );
}

TEST( EntityCombatModelTest, SetAttackRange_UpdatesAttackRange ) {
    Engine::EntityCombatModel combat;
    combat.setAttackRange( 4 );

    EXPECT_EQ( combat.attackRange(), 4 );
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
