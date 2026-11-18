#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include <MMORPGServer/Server/Runtime/World/Action/WorldCombatAttackPlanner.h>

namespace {

constexpr int ORIGIN_X = 10;
constexpr int ORIGIN_Y = 10;
constexpr int TICK_RATE = 20;

Engine::AttackModel attackWith( std::vector<Engine::AttackCellModel> cells ) {
    Engine::AttackStepModel step;
    step.setCells( cells );

    Engine::AttackModel attack;
    attack.setSteps( { step } );

    return attack;
}

class FacingCase {
public:
    FacingCase( std::string name, int dx, int dy, std::vector<std::pair<int, int>> expected ) :
        name( std::move( name ) ),
        expected( std::move( expected ) ),
        dx( dx ),
        dy( dy ) {
    }

    std::string name;
    std::vector<std::pair<int, int>> expected;
    int dx;
    int dy;
};

class AreaFacingTest : public ::testing::TestWithParam<FacingCase> {};

TEST_P( AreaFacingTest, RotatesFrontAndRightCells ) {
    const Engine::AttackModel attack = attackWith( { Engine::AttackCellModel( 1, 0 ), Engine::AttackCellModel( 1, 1 ) } );

    const std::vector<Server::WorldCombatStrikeModel> strikes = Server::WorldCombatAttackPlanner::plan( attack, ORIGIN_X, ORIGIN_Y, GetParam().dx, GetParam().dy, TICK_RATE );

    ASSERT_EQ( strikes.size(), 1u );
    EXPECT_EQ( strikes[ 0 ].cells(), GetParam().expected );
}

INSTANTIATE_TEST_SUITE_P(
    WorldCombatAttackPlanner,
    AreaFacingTest,
    ::testing::Values(
        FacingCase( "Cima", 0, -1, { { 10, 9 }, { 11, 9 } } ),
        FacingCase( "Baixo", 0, 1, { { 10, 11 }, { 9, 11 } } ),
        FacingCase( "Direita", 1, 0, { { 11, 10 }, { 11, 11 } } ),
        FacingCase( "Esquerda", -1, 0, { { 9, 10 }, { 9, 9 } } ),
        FacingCase( "CimaDireita", 1, -1, { { 11, 9 }, { 11, 10 } } ),
        FacingCase( "BaixoDireita", 1, 1, { { 11, 11 }, { 10, 11 } } ),
        FacingCase( "BaixoEsquerda", -1, 1, { { 9, 11 }, { 9, 10 } } ),
        FacingCase( "CimaEsquerda", -1, -1, { { 9, 9 }, { 10, 9 } } ),
        FacingCase( "AlvoDistanteComEixoVerticalDominanteViraBaixo", 1, 3, { { 10, 11 }, { 9, 11 } } ),
        FacingCase( "AlvoDistanteComEixoHorizontalDominanteViraDireita", 3, 1, { { 11, 10 }, { 11, 11 } } ),
        FacingCase( "SemDirecaoOlhaPraCima", 0, 0, { { 10, 9 }, { 11, 9 } } ) ),
    []( const ::testing::TestParamInfo<FacingCase>& info ) { return info.param.name; } );

} // namespace

TEST( WorldCombatAttackPlannerTest, LinhaNaDiagonalContinuaDiagonal ) {
    const Engine::AttackModel attack = attackWith( { Engine::AttackCellModel( 1, 0 ), Engine::AttackCellModel( 2, 0 ) } );

    const std::vector<Server::WorldCombatStrikeModel> strikes = Server::WorldCombatAttackPlanner::plan( attack, ORIGIN_X, ORIGIN_Y, 1, -1, TICK_RATE );

    ASSERT_EQ( strikes.size(), 1u );
    const std::vector<std::pair<int, int>> expected = { { 11, 9 }, { 12, 8 } };
    EXPECT_EQ( strikes[ 0 ].cells(), expected );
}

TEST( WorldCombatAttackPlannerTest, FormaSimetricaAcertaAsMesmasCelulasEmQualquerDirecao ) {
    const Engine::AttackModel attack = attackWith( { Engine::AttackCellModel( 1, -1 ), Engine::AttackCellModel( 1, 0 ), Engine::AttackCellModel( 1, 1 ), Engine::AttackCellModel( 0, -1 ), Engine::AttackCellModel( 0, 1 ), Engine::AttackCellModel( -1, -1 ), Engine::AttackCellModel( -1, 0 ), Engine::AttackCellModel( -1, 1 ) } );

    std::vector<std::vector<std::pair<int, int>>> results;
    for ( const std::pair<int, int>& direction : std::vector<std::pair<int, int>>{ { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 } } ) {
        std::vector<std::pair<int, int>> cells = Server::WorldCombatAttackPlanner::plan( attack, ORIGIN_X, ORIGIN_Y, direction.first, direction.second, TICK_RATE )[ 0 ].cells();
        std::sort( cells.begin(), cells.end() );
        results.push_back( cells );
    }

    ASSERT_EQ( results[ 0 ].size(), 8u );
    for ( const std::vector<std::pair<int, int>>& cells : results ) {
        EXPECT_EQ( cells, results[ 0 ] );
    }
}

TEST( WorldCombatAttackPlannerTest, RotacaoNaDiagonalNaoPerdeNemRepeteCelulas ) {
    std::vector<Engine::AttackCellModel> block;
    for ( int forward = 1; forward <= 3; ++forward ) {
        for ( int lateral = -2; lateral <= 2; ++lateral ) {
            block.push_back( Engine::AttackCellModel( forward, lateral ) );
        }
    }

    const Engine::AttackModel attack = attackWith( block );

    for ( const std::pair<int, int>& direction : std::vector<std::pair<int, int>>{ { 1, -1 }, { 1, 1 }, { -1, 1 }, { -1, -1 } } ) {
        std::vector<std::pair<int, int>> cells = Server::WorldCombatAttackPlanner::plan( attack, ORIGIN_X, ORIGIN_Y, direction.first, direction.second, TICK_RATE )[ 0 ].cells();
        std::sort( cells.begin(), cells.end() );

        EXPECT_EQ( cells.size(), block.size() );
        EXPECT_EQ( std::adjacent_find( cells.begin(), cells.end() ), cells.end() );
    }
}

TEST( WorldCombatAttackPlannerTest, CelulasSaemOrdenadasPorDistanciaDaOrigem ) {
    const Engine::AttackModel attack = attackWith( { Engine::AttackCellModel( 3, 0 ), Engine::AttackCellModel( 1, 0 ), Engine::AttackCellModel( 2, 0 ) } );

    const std::vector<Server::WorldCombatStrikeModel> strikes = Server::WorldCombatAttackPlanner::plan( attack, ORIGIN_X, ORIGIN_Y, 0, -1, TICK_RATE );

    const std::vector<std::pair<int, int>> expected = { { 10, 9 }, { 10, 8 }, { 10, 7 } };
    EXPECT_EQ( strikes[ 0 ].cells(), expected );
}

TEST( WorldCombatAttackPlannerTest, PassoGeraUmGolpeComCastMaisDelayEMultiplicadorDeDano ) {
    Engine::AttackStepModel step;
    step.setCells( { Engine::AttackCellModel( 1, 0 ) } );
    step.setDelaySeconds( 0.1 );
    step.setDamageMultiplier( 2.0 );

    Engine::AttackModel attack;
    attack.setCastSeconds( 0.5 );
    attack.setSteps( { step } );

    const std::vector<Server::WorldCombatStrikeModel> strikes = Server::WorldCombatAttackPlanner::plan( attack, ORIGIN_X, ORIGIN_Y, 0, -1, TICK_RATE );

    ASSERT_EQ( strikes.size(), 1u );
    EXPECT_DOUBLE_EQ( strikes[ 0 ].seconds(), 0.6 );
    EXPECT_EQ( strikes[ 0 ].ticks(), 12 );
    EXPECT_DOUBLE_EQ( strikes[ 0 ].damageMultiplier(), 2.0 );
}

TEST( WorldCombatAttackPlannerTest, PassosViramGolpesSeparadosComSeusAtrasos ) {
    Engine::AttackStepModel near;
    near.setCells( { Engine::AttackCellModel( 1, 0 ) } );

    Engine::AttackStepModel far;
    far.setCells( { Engine::AttackCellModel( 2, 0 ) } );
    far.setDelaySeconds( 0.2 );

    Engine::AttackModel attack;
    attack.setSteps( { near, far } );

    const std::vector<Server::WorldCombatStrikeModel> strikes = Server::WorldCombatAttackPlanner::plan( attack, ORIGIN_X, ORIGIN_Y, 0, -1, TICK_RATE );

    ASSERT_EQ( strikes.size(), 2u );
    EXPECT_EQ( strikes[ 0 ].ticks(), 10 );
    EXPECT_EQ( strikes[ 1 ].ticks(), 14 );
    EXPECT_EQ( strikes[ 1 ].cells()[ 0 ], std::make_pair( 10, 8 ) );
}
