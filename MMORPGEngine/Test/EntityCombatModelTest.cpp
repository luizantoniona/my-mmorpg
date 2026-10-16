#include <gtest/gtest.h>

#include <string>

#include <MMORPGEngine/Entity/EntityCombatModel.h>

namespace {

class IsReadyCase {
public:
    IsReadyCase( std::string name, double cooldownSeconds, int counter, int tickRate, bool expected ) :
        name( std::move( name ) ),
        cooldownSeconds( cooldownSeconds ),
        counter( counter ),
        tickRate( tickRate ),
        expected( expected ) {
    }

    std::string name;
    double cooldownSeconds;
    int counter;
    int tickRate;
    bool expected;
};

class DefaultActionCase {
public:
    DefaultActionCase( std::string name, Engine::CombatActionEnum action, bool expected ) :
        name( std::move( name ) ),
        action( action ),
        expected( expected ) {
    }

    std::string name;
    Engine::CombatActionEnum action;
    bool expected;
};

class EntityCombatModelIsReadyTest : public ::testing::TestWithParam<IsReadyCase> {};
class EntityCombatModelDefaultActionTest : public ::testing::TestWithParam<DefaultActionCase> {};

} // namespace

TEST( EntityCombatModelTest, DefaultConstructed_IsNotReady ) {
    const Engine::EntityCombatModel combat;

    EXPECT_FALSE( combat.isReady( 20 ) );
}

TEST_P( EntityCombatModelDefaultActionTest, DefaultConstructed_ActionAvailability ) {
    const Engine::EntityCombatModel combat;

    EXPECT_EQ( combat.isActionAvailable( GetParam().action ), GetParam().expected );
}

INSTANTIATE_TEST_SUITE_P(
    Actions,
    EntityCombatModelDefaultActionTest,
    ::testing::Values(
        DefaultActionCase( "Attack_IsAvailable", Engine::CombatActionEnum::ATTACK, true ),
        DefaultActionCase( "Dodge_IsAvailable", Engine::CombatActionEnum::DODGE, true ),
        DefaultActionCase( "Block_IsNotAvailable", Engine::CombatActionEnum::BLOCK, false ) ),
    []( const ::testing::TestParamInfo<DefaultActionCase>& info ) { return info.param.name; } );

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

TEST_P( EntityCombatModelIsReadyTest, IsReady_ReturnsExpected ) {
    Engine::EntityCombatModel combat;
    combat.setCooldownSeconds( GetParam().cooldownSeconds );
    combat.setCounter( GetParam().counter );

    EXPECT_EQ( combat.isReady( GetParam().tickRate ), GetParam().expected );
}

INSTANTIATE_TEST_SUITE_P(
    Counters,
    EntityCombatModelIsReadyTest,
    ::testing::Values(
        IsReadyCase( "CounterBelowCooldown", 1.0, 19, 20, false ),
        IsReadyCase( "CounterEqualsCooldown", 1.0, 20, 20, true ),
        IsReadyCase( "CounterAboveCooldown", 1.0, 25, 20, true ) ),
    []( const ::testing::TestParamInfo<IsReadyCase>& info ) { return info.param.name; } );

TEST( EntityCombatModelTest, CooldownTicks_ConvertsSecondsUsingTickRate ) {
    Engine::EntityCombatModel combat;
    combat.setCooldownSeconds( 1.5 );

    EXPECT_EQ( combat.cooldownTicks( 20 ), 30 );
}
