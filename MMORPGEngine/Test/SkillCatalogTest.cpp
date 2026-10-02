#include <gtest/gtest.h>

#include <MMORPGEngine/Data/Skill/SkillCatalog.h>

TEST( SkillCatalogTest, AddTree_ThenGet_ReturnsSameTree ) {
    Engine::SkillCatalog catalog;

    Engine::SkillTreeModel tree;
    tree.setItemType( "SWORD" );
    catalog.addTree( tree );

    ASSERT_NE( catalog.tree( "SWORD" ), nullptr );
    EXPECT_EQ( catalog.tree( "SWORD" )->itemType(), "SWORD" );
}

TEST( SkillCatalogTest, Tree_UnknownItemType_ReturnsNullptr ) {
    const Engine::SkillCatalog catalog;

    EXPECT_EQ( catalog.tree( "GHOST" ), nullptr );
}

TEST( SkillCatalogTest, AddTree_DuplicateItemType_KeepsFirstEntry ) {
    Engine::SkillCatalog catalog;

    Engine::SkillTreeModel first;
    first.setItemType( "SWORD" );
    Engine::SkillNodeModel firstNode;
    firstNode.setType( 1 );
    firstNode.setName( "Sword Mastery" );
    first.addNode( firstNode );
    catalog.addTree( first );

    Engine::SkillTreeModel second;
    second.setItemType( "SWORD" );
    Engine::SkillNodeModel secondNode;
    secondNode.setType( 1 );
    secondNode.setName( "Axe Mastery" );
    second.addNode( secondNode );
    catalog.addTree( second );

    ASSERT_NE( catalog.tree( "SWORD" )->node( 1 ), nullptr );
    EXPECT_EQ( catalog.tree( "SWORD" )->node( 1 )->name(), "Sword Mastery" );
}

TEST( SkillCatalogTest, Trees_ReturnsAllAddedEntries ) {
    Engine::SkillCatalog catalog;

    Engine::SkillTreeModel first;
    first.setItemType( "SWORD" );
    catalog.addTree( first );

    Engine::SkillTreeModel second;
    second.setItemType( "AXE" );
    catalog.addTree( second );

    EXPECT_EQ( catalog.trees().size(), 2u );
}
