#include <gtest/gtest.h>

#include <MMORPGEngine/Data/Skill/SkillTreeModel.h>

TEST( SkillTreeModelTest, AddNode_ThenGet_ReturnsSameNode ) {
    Engine::SkillTreeModel tree;

    Engine::SkillNodeModel node;
    node.setType( 1 );
    node.setName( "Sword Mastery" );
    tree.addNode( node );

    ASSERT_NE( tree.node( 1 ), nullptr );
    EXPECT_EQ( tree.node( 1 )->name(), "Sword Mastery" );
}

TEST( SkillTreeModelTest, Node_UnknownType_ReturnsNullptr ) {
    const Engine::SkillTreeModel tree;

    EXPECT_EQ( tree.node( 99 ), nullptr );
}

TEST( SkillTreeModelTest, AddNode_DuplicateType_KeepsFirstEntry ) {
    Engine::SkillTreeModel tree;

    Engine::SkillNodeModel first;
    first.setType( 1 );
    first.setName( "Sword Mastery" );
    tree.addNode( first );

    Engine::SkillNodeModel second;
    second.setType( 1 );
    second.setName( "Whirlwind Strike" );
    tree.addNode( second );

    EXPECT_EQ( tree.node( 1 )->name(), "Sword Mastery" );
}

TEST( SkillTreeModelTest, Nodes_ReturnsAllAddedEntries ) {
    Engine::SkillTreeModel tree;

    Engine::SkillNodeModel first;
    first.setType( 1 );
    tree.addNode( first );

    Engine::SkillNodeModel second;
    second.setType( 2 );
    tree.addNode( second );

    EXPECT_EQ( tree.nodes().size(), 2u );
}
