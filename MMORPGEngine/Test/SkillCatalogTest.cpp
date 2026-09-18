#include <gtest/gtest.h>

#include <MMORPGEngine/Data/Skill/SkillCatalog.h>

TEST( SkillCatalogTest, AddTree_ThenGet_ReturnsSameTree ) {
    Engine::SkillCatalog catalog;

    Engine::SkillTreeModel tree;
    tree.setItemType( 1 );
    catalog.addTree( tree );

    ASSERT_NE( catalog.tree( 1 ), nullptr );
    EXPECT_EQ( catalog.tree( 1 )->itemType(), 1u );
}

TEST( SkillCatalogTest, Tree_UnknownItemType_ReturnsNullptr ) {
    const Engine::SkillCatalog catalog;

    EXPECT_EQ( catalog.tree( 99 ), nullptr );
}

TEST( SkillCatalogTest, AddTree_DuplicateItemType_KeepsFirstEntry ) {
    Engine::SkillCatalog catalog;

    Engine::SkillTreeModel first;
    first.setItemType( 1 );
    Engine::SkillNodeModel firstNode;
    firstNode.setType( 1 );
    firstNode.setName( "Sword Mastery" );
    first.addNode( firstNode );
    catalog.addTree( first );

    Engine::SkillTreeModel second;
    second.setItemType( 1 );
    Engine::SkillNodeModel secondNode;
    secondNode.setType( 1 );
    secondNode.setName( "Axe Mastery" );
    second.addNode( secondNode );
    catalog.addTree( second );

    ASSERT_NE( catalog.tree( 1 )->node( 1 ), nullptr );
    EXPECT_EQ( catalog.tree( 1 )->node( 1 )->name(), "Sword Mastery" );
}

TEST( SkillCatalogTest, Trees_ReturnsAllAddedEntries ) {
    Engine::SkillCatalog catalog;

    Engine::SkillTreeModel first;
    first.setItemType( 1 );
    catalog.addTree( first );

    Engine::SkillTreeModel second;
    second.setItemType( 2 );
    catalog.addTree( second );

    EXPECT_EQ( catalog.trees().size(), 2u );
}
