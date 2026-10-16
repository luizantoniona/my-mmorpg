#include <gtest/gtest.h>

#include <string>

#include <MMORPGEngine/Entity/EntityMovementModel.h>

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

class CooldownTicksCase {
public:
    CooldownTicksCase( std::string name, double cooldownSeconds, int tickRate, int expected ) :
        name( std::move( name ) ),
        cooldownSeconds( cooldownSeconds ),
        tickRate( tickRate ),
        expected( expected ) {
    }

    std::string name;
    double cooldownSeconds;
    int tickRate;
    int expected;
};

class EntityMovementModelIsReadyTest : public ::testing::TestWithParam<IsReadyCase> {};
class EntityMovementModelCooldownTicksTest : public ::testing::TestWithParam<CooldownTicksCase> {};

} // namespace

TEST( EntityMovementModelTest, DefaultConstructed_IsNotReady ) {
    const Engine::EntityMovementModel movement;

    EXPECT_FALSE( movement.isReady( 20 ) );
}

TEST_P( EntityMovementModelIsReadyTest, IsReady_ReturnsExpected ) {
    Engine::EntityMovementModel movement;
    movement.setCooldownSeconds( GetParam().cooldownSeconds );
    movement.setCounter( GetParam().counter );

    EXPECT_EQ( movement.isReady( GetParam().tickRate ), GetParam().expected );
}

INSTANTIATE_TEST_SUITE_P(
    Counters,
    EntityMovementModelIsReadyTest,
    ::testing::Values(
        IsReadyCase( "CounterBelowCooldown", 1.0, 19, 20, false ),
        IsReadyCase( "CounterEqualsCooldown", 1.0, 20, 20, true ),
        IsReadyCase( "CounterAboveCooldown", 1.0, 25, 20, true ),
        IsReadyCase( "SameCooldownHigherTickRate_NeedsMoreTicks", 1.0, 20, 30, false ) ),
    []( const ::testing::TestParamInfo<IsReadyCase>& info ) { return info.param.name; } );

TEST_P( EntityMovementModelCooldownTicksTest, CooldownTicks_ConvertsSecondsUsingTickRate ) {
    Engine::EntityMovementModel movement;
    movement.setCooldownSeconds( GetParam().cooldownSeconds );

    EXPECT_EQ( movement.cooldownTicks( GetParam().tickRate ), GetParam().expected );
}

INSTANTIATE_TEST_SUITE_P(
    TickRates,
    EntityMovementModelCooldownTicksTest,
    ::testing::Values(
        CooldownTicksCase( "OneAndHalfSecondAt20", 1.5, 20, 30 ),
        CooldownTicksCase( "OneSecondAt30", 1.0, 30, 30 ) ),
    []( const ::testing::TestParamInfo<CooldownTicksCase>& info ) { return info.param.name; } );
