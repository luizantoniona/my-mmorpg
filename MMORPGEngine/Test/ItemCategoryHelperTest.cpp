#include <gtest/gtest.h>

#include <set>
#include <string>

#include <MMORPGEngine/Data/Item/ItemCategoryHelper.h>

namespace {

class GrantedActionsCase {
public:
    GrantedActionsCase( std::string name, Engine::ItemCategoryEnum category, std::set<Engine::CombatActionEnum> expected ) :
        name( std::move( name ) ),
        category( category ),
        expected( std::move( expected ) ) {
    }

    std::string name;
    Engine::ItemCategoryEnum category;
    std::set<Engine::CombatActionEnum> expected;
};

class ItemCategoryHelperGrantedActionsTest : public ::testing::TestWithParam<GrantedActionsCase> {};

} // namespace

TEST_P( ItemCategoryHelperGrantedActionsTest, GrantedActions_ReturnsExpectedActions ) {
    EXPECT_EQ( Engine::ItemCategoryHelper::grantedActions( GetParam().category ), GetParam().expected );
}

INSTANTIATE_TEST_SUITE_P(
    Categories,
    ItemCategoryHelperGrantedActionsTest,
    ::testing::Values(
        GrantedActionsCase( "Weapon_GrantsAttack", Engine::ItemCategoryEnum::WEAPON, { Engine::CombatActionEnum::ATTACK } ),
        GrantedActionsCase( "Shield_GrantsBlock", Engine::ItemCategoryEnum::SHIELD, { Engine::CombatActionEnum::BLOCK } ),
        GrantedActionsCase( "Offhand_GrantsNothing", Engine::ItemCategoryEnum::OFFHAND, {} ),
        GrantedActionsCase( "Armor_GrantsNothing", Engine::ItemCategoryEnum::ARMOR, {} ),
        GrantedActionsCase( "Unknown_GrantsNothing", Engine::ItemCategoryEnum::UNKNOWN, {} ) ),
    []( const ::testing::TestParamInfo<GrantedActionsCase>& info ) { return info.param.name; } );
