#include <gtest/gtest.h>

#include <MMORPGEngine/Data/Monster/MonsterCatalog.h>

TEST( MonsterCatalogTest, AddMonster_ThenGet_ReturnsSameMonster ) {
    Engine::MonsterCatalog catalog;

    Engine::MonsterModel monster;
    monster.setType( 1 );
    monster.setName( "Goblin" );
    catalog.addMonster( monster );

    ASSERT_NE( catalog.monster( 1 ), nullptr );
    EXPECT_EQ( catalog.monster( 1 )->name(), "Goblin" );
}

TEST( MonsterCatalogTest, Monster_UnknownType_ReturnsNullptr ) {
    const Engine::MonsterCatalog catalog;

    EXPECT_EQ( catalog.monster( 99 ), nullptr );
}

TEST( MonsterCatalogTest, AddMonster_DuplicateType_KeepsFirstEntry ) {
    Engine::MonsterCatalog catalog;

    Engine::MonsterModel first;
    first.setType( 1 );
    first.setName( "Goblin" );
    catalog.addMonster( first );

    Engine::MonsterModel second;
    second.setType( 1 );
    second.setName( "Rat" );
    catalog.addMonster( second );

    EXPECT_EQ( catalog.monster( 1 )->name(), "Goblin" );
}

TEST( MonsterCatalogTest, Monsters_ReturnsAllAddedEntries ) {
    Engine::MonsterCatalog catalog;

    Engine::MonsterModel first;
    first.setType( 1 );
    catalog.addMonster( first );

    Engine::MonsterModel second;
    second.setType( 2 );
    catalog.addMonster( second );

    EXPECT_EQ( catalog.monsters().size(), 2u );
}
