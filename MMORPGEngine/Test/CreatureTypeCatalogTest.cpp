#include <gtest/gtest.h>

#include <MMORPGEngine/Data/Creature/CreatureTypeCatalog.h>

TEST( CreatureTypeCatalogTest, AddCreatureType_ThenGet_ReturnsSameCreatureType ) {
    Engine::CreatureTypeCatalog catalog;

    Engine::CreatureTypeModel creatureType;
    creatureType.setType( 1 );
    creatureType.setName( "Goblin" );
    catalog.addCreatureType( creatureType );

    ASSERT_NE( catalog.creatureType( 1 ), nullptr );
    EXPECT_EQ( catalog.creatureType( 1 )->name(), "Goblin" );
}

TEST( CreatureTypeCatalogTest, CreatureType_UnknownType_ReturnsNullptr ) {
    const Engine::CreatureTypeCatalog catalog;

    EXPECT_EQ( catalog.creatureType( 99 ), nullptr );
}

TEST( CreatureTypeCatalogTest, AddCreatureType_DuplicateType_KeepsFirstEntry ) {
    Engine::CreatureTypeCatalog catalog;

    Engine::CreatureTypeModel first;
    first.setType( 1 );
    first.setName( "Goblin" );
    catalog.addCreatureType( first );

    Engine::CreatureTypeModel second;
    second.setType( 1 );
    second.setName( "Rat" );
    catalog.addCreatureType( second );

    EXPECT_EQ( catalog.creatureType( 1 )->name(), "Goblin" );
}

TEST( CreatureTypeCatalogTest, CreatureTypes_ReturnsAllAddedEntries ) {
    Engine::CreatureTypeCatalog catalog;

    Engine::CreatureTypeModel first;
    first.setType( 1 );
    catalog.addCreatureType( first );

    Engine::CreatureTypeModel second;
    second.setType( 2 );
    catalog.addCreatureType( second );

    EXPECT_EQ( catalog.creatureTypes().size(), 2u );
}
