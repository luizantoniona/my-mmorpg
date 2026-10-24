#include <gtest/gtest.h>

#include <string>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Attack/AttackFactory.h>

namespace {

std::string stepJson( const std::string& shape, const std::string& delay = "0.0", const std::string& damage = "1.0" ) {
    return R"({"Delay":)" + delay + R"(,"Damage":)" + damage + R"(,"Shape":)" + shape + "}";
}

std::string attackJson( const std::string& steps, const std::string& cast = "0.5", const std::string& hitMode = "FIRST", const std::string& extra = "" ) {
    return R"({"CastSeconds":)" + cast + R"(,"HitMode":")" + hitMode + R"(",)" + extra + R"("Steps":[)" + steps + "]}";
}

const std::string FRONT_CELL = R"(["X","@"])";

class InvalidAttackCase {
public:
    InvalidAttackCase( std::string name, std::string json ) :
        name( std::move( name ) ),
        json( std::move( json ) ) {
    }

    std::string name;
    std::string json;
};

class InvalidAttackTest : public ::testing::TestWithParam<InvalidAttackCase> {};

TEST_P( InvalidAttackTest, Rejected ) {
    Engine::AttackModel attack;

    EXPECT_FALSE( Engine::AttackFactory::fromJson( Engine::JsonHelper::parseJsonString( GetParam().json ), attack ) );
}

INSTANTIATE_TEST_SUITE_P(
    AttackFactory,
    InvalidAttackTest,
    ::testing::Values(
        InvalidAttackCase( "Vazio", R"({})" ),
        InvalidAttackCase( "SemCastSeconds", R"({"HitMode":"FIRST","Steps":[)" + stepJson( FRONT_CELL ) + "]}" ),
        InvalidAttackCase( "SemHitMode", R"({"CastSeconds":0.5,"Steps":[)" + stepJson( FRONT_CELL ) + "]}" ),
        InvalidAttackCase( "SemSteps", R"({"CastSeconds":0.5,"HitMode":"FIRST"})" ),
        InvalidAttackCase( "StepsVazio", attackJson( "" ) ),
        InvalidAttackCase( "StepsComTipoErrado", R"({"CastSeconds":0.5,"HitMode":"FIRST","Steps":"X"})" ),
        InvalidAttackCase( "ShapeNoTopoDoAtaque", attackJson( stepJson( FRONT_CELL ), "0.5", "FIRST", R"("Shape":["X","@"],)" ) ),
        InvalidAttackCase( "PassoSemDelay", attackJson( R"({"Damage":1.0,"Shape":["X","@"]})" ) ),
        InvalidAttackCase( "PassoSemDamage", attackJson( R"({"Delay":0.0,"Shape":["X","@"]})" ) ),
        InvalidAttackCase( "PassoSemShape", attackJson( R"({"Delay":0.0,"Damage":1.0})" ) ),
        InvalidAttackCase( "ShapeSemOrigem", attackJson( stepJson( R"(["X","X"])" ) ) ),
        InvalidAttackCase( "ShapeComDuasOrigens", attackJson( stepJson( R"(["X@","@"])" ) ) ),
        InvalidAttackCase( "ShapeComCaractereInvalido", attackJson( stepJson( R"(["Z","@"])" ) ) ),
        InvalidAttackCase( "ShapeComDigito", attackJson( stepJson( R"(["2","1","@"])" ) ) ),
        InvalidAttackCase( "ShapeSoOrigem", attackJson( stepJson( R"(["@"])" ) ) ),
        InvalidAttackCase( "HitModeDesconhecido", attackJson( stepJson( FRONT_CELL ), "0.5", "SOME" ) ),
        InvalidAttackCase( "HitModeComTipoErrado", R"({"CastSeconds":0.5,"HitMode":1,"Steps":[)" + stepJson( FRONT_CELL ) + "]}" ),
        InvalidAttackCase( "CastSecondsZero", attackJson( stepJson( FRONT_CELL ), "0" ) ),
        InvalidAttackCase( "CastSecondsComTipoErrado", attackJson( stepJson( FRONT_CELL ), R"("1")" ) ),
        InvalidAttackCase( "DelayNegativo", attackJson( stepJson( FRONT_CELL, "-1.0" ) ) ),
        InvalidAttackCase( "DamageZero", attackJson( stepJson( FRONT_CELL, "0.0", "0.0" ) ) ),
        InvalidAttackCase( "DamageNegativo", attackJson( stepJson( FRONT_CELL, "0.0", "-1.0" ) ) ),
        InvalidAttackCase( "UmPassoValidoEOutroInvalido", attackJson( stepJson( FRONT_CELL ) + "," + stepJson( R"(["X"])" ) ) ) ),
    []( const ::testing::TestParamInfo<InvalidAttackCase>& info ) { return info.param.name; } );

Engine::AttackModel parseValid( const std::string& json ) {
    Engine::AttackModel attack;
    EXPECT_TRUE( Engine::AttackFactory::fromJson( Engine::JsonHelper::parseJsonString( json ), attack ) );

    return attack;
}

} // namespace

TEST( AttackFactoryTest, UmPassoViraUmaCelulaAFrenteComPadroesDoAtaque ) {
    const Engine::AttackModel attack = parseValid( attackJson( stepJson( FRONT_CELL ) ) );

    ASSERT_EQ( attack.steps().size(), 1u );
    ASSERT_EQ( attack.steps()[ 0 ].cells().size(), 1u );
    EXPECT_EQ( attack.steps()[ 0 ].cells()[ 0 ].forward(), 1 );
    EXPECT_EQ( attack.steps()[ 0 ].cells()[ 0 ].lateral(), 0 );
    EXPECT_DOUBLE_EQ( attack.steps()[ 0 ].delaySeconds(), 0.0 );
    EXPECT_DOUBLE_EQ( attack.steps()[ 0 ].damageMultiplier(), 1.0 );
    EXPECT_EQ( attack.hitMode(), Engine::AttackHitModeEnum::FIRST );
    EXPECT_DOUBLE_EQ( attack.castSeconds(), 0.5 );
}

TEST( AttackFactoryTest, LateralPositivaEhADireitaDaOrigem ) {
    const Engine::AttackModel attack = parseValid( attackJson( stepJson( R"(["XX"," @"])" ) ) );

    ASSERT_EQ( attack.steps()[ 0 ].cells().size(), 2u );
    EXPECT_EQ( attack.steps()[ 0 ].cells()[ 0 ].forward(), 1 );
    EXPECT_EQ( attack.steps()[ 0 ].cells()[ 0 ].lateral(), -1 );
    EXPECT_EQ( attack.steps()[ 0 ].cells()[ 1 ].forward(), 1 );
    EXPECT_EQ( attack.steps()[ 0 ].cells()[ 1 ].lateral(), 0 );
}

TEST( AttackFactoryTest, EmVoltaDaOrigemLeTodasAsOitoCelulas ) {
    const Engine::AttackModel attack = parseValid( attackJson( stepJson( R"(["XXX","X@X","XXX"])" ), "1.2", "ALL" ) );

    ASSERT_EQ( attack.steps().size(), 1u );
    EXPECT_EQ( attack.steps()[ 0 ].cells().size(), 8u );
}

TEST( AttackFactoryTest, PassosViramOrdemEDelayPropriosDeCadaUm ) {
    const Engine::AttackModel attack = parseValid( attackJson( stepJson( FRONT_CELL ) + "," + stepJson( R"(["X","X","@"])", "0.2" ) + "," + stepJson( R"(["XXX","...","..@"])", "0.4" ), "0.5", "ALL" ) );

    ASSERT_EQ( attack.steps().size(), 3u );
    EXPECT_DOUBLE_EQ( attack.steps()[ 0 ].delaySeconds(), 0.0 );
    EXPECT_DOUBLE_EQ( attack.steps()[ 1 ].delaySeconds(), 0.2 );
    EXPECT_DOUBLE_EQ( attack.steps()[ 2 ].delaySeconds(), 0.4 );
    EXPECT_EQ( attack.steps()[ 1 ].cells().size(), 2u );
    EXPECT_EQ( attack.steps()[ 2 ].cells().size(), 3u );
}

TEST( AttackFactoryTest, PassoLeDelayEDano ) {
    const Engine::AttackModel attack = parseValid( attackJson( stepJson( FRONT_CELL, "0.1", "1.5" ), "0.8", "ALL" ) );

    EXPECT_DOUBLE_EQ( attack.castSeconds(), 0.8 );
    EXPECT_EQ( attack.hitMode(), Engine::AttackHitModeEnum::ALL );

    ASSERT_EQ( attack.steps().size(), 1u );
    EXPECT_DOUBLE_EQ( attack.steps()[ 0 ].delaySeconds(), 0.1 );
    EXPECT_DOUBLE_EQ( attack.steps()[ 0 ].damageMultiplier(), 1.5 );
}

TEST( AttackFactoryTest, BasicEhUmaCelulaAFrente ) {
    const Engine::AttackModel attack = Engine::AttackModel::basic();

    ASSERT_EQ( attack.steps().size(), 1u );
    ASSERT_EQ( attack.steps()[ 0 ].cells().size(), 1u );
    EXPECT_EQ( attack.steps()[ 0 ].cells()[ 0 ].forward(), 1 );
    EXPECT_EQ( attack.steps()[ 0 ].cells()[ 0 ].lateral(), 0 );
}
