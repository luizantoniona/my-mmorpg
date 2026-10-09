#include <gtest/gtest.h>

#include <MMORPGEngine/Data/Creature/CreatureTypeCatalog.h>

TEST( CreatureTypeCatalogTest, AddCreatureType_ThenGet_ReturnsSameCreatureType ) {
    Engine::CreatureTypeCatalog catalog;

    Engine::CreatureTypeModel creatureType;
    creatureType.setType( "GOBLIN" );
    creatureType.setName( "Goblin" );
    catalog.addCreatureType( creatureType );

    ASSERT_NE( catalog.creatureType( "GOBLIN" ), nullptr );
    EXPECT_EQ( catalog.creatureType( "GOBLIN" )->name(), "Goblin" );
}

TEST( CreatureTypeCatalogTest, CreatureType_UnknownType_ReturnsNullptr ) {
    const Engine::CreatureTypeCatalog catalog;

    EXPECT_EQ( catalog.creatureType( "UNKNOWN" ), nullptr );
}

TEST( CreatureTypeCatalogTest, AddCreatureType_DuplicateType_KeepsFirstEntry ) {
    Engine::CreatureTypeCatalog catalog;

    Engine::CreatureTypeModel first;
    first.setType( "GOBLIN" );
    first.setName( "Goblin" );
    catalog.addCreatureType( first );

    Engine::CreatureTypeModel second;
    second.setType( "GOBLIN" );
    second.setName( "Rat" );
    catalog.addCreatureType( second );

    EXPECT_EQ( catalog.creatureType( "GOBLIN" )->name(), "Goblin" );
}

TEST( CreatureTypeCatalogTest, CreatureTypes_ReturnsAllAddedEntries ) {
    Engine::CreatureTypeCatalog catalog;

    Engine::CreatureTypeModel first;
    first.setType( "GOBLIN" );
    catalog.addCreatureType( first );

    Engine::CreatureTypeModel second;
    second.setType( "RAT" );
    catalog.addCreatureType( second );

    EXPECT_EQ( catalog.creatureTypes().size(), 2u );
}
