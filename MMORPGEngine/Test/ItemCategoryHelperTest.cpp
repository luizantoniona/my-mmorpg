#include <gtest/gtest.h>

#include <set>

#include <MMORPGEngine/Data/Item/ItemCategoryHelper.h>

TEST( ItemCategoryHelperTest, GrantedActions_Weapon_GrantsAttack ) {
    EXPECT_EQ( Engine::ItemCategoryHelper::grantedActions( Engine::ItemCategoryEnum::WEAPON ), std::set<Engine::CombatActionEnum>( { Engine::CombatActionEnum::ATTACK } ) );
}

TEST( ItemCategoryHelperTest, GrantedActions_Shield_GrantsBlock ) {
    EXPECT_EQ( Engine::ItemCategoryHelper::grantedActions( Engine::ItemCategoryEnum::SHIELD ), std::set<Engine::CombatActionEnum>( { Engine::CombatActionEnum::BLOCK } ) );
}

TEST( ItemCategoryHelperTest, GrantedActions_OffhandArmorAndUnknown_GrantNothing ) {
    EXPECT_TRUE( Engine::ItemCategoryHelper::grantedActions( Engine::ItemCategoryEnum::OFFHAND ).empty() );
    EXPECT_TRUE( Engine::ItemCategoryHelper::grantedActions( Engine::ItemCategoryEnum::ARMOR ).empty() );
    EXPECT_TRUE( Engine::ItemCategoryHelper::grantedActions( Engine::ItemCategoryEnum::UNKNOWN ).empty() );
}
