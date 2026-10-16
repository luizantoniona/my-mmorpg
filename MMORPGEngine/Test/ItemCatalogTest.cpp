#include <gtest/gtest.h>

#include <MMORPGEngine/Data/Item/ItemCatalog.h>

TEST( ItemCatalogTest, AddItem_ThenGet_ReturnsSameItem ) {
    Engine::ItemCatalog catalog;

    Engine::ItemModel item;
    item.setId( "IRON_SWORD" );
    item.setIdItemType( "SWORD" );
    item.setName( "Iron Sword" );
    catalog.addItem( item );

    ASSERT_NE( catalog.item( "IRON_SWORD" ), nullptr );
    EXPECT_EQ( catalog.item( "IRON_SWORD" )->name(), "Iron Sword" );
    EXPECT_EQ( catalog.item( "IRON_SWORD" )->idItemType(), "SWORD" );
}

TEST( ItemCatalogTest, Item_UnknownId_ReturnsNullptr ) {
    const Engine::ItemCatalog catalog;

    EXPECT_EQ( catalog.item( "MISSING" ), nullptr );
}

TEST( ItemCatalogTest, Item_IdIsCaseSensitive ) {
    Engine::ItemCatalog catalog;

    Engine::ItemModel item;
    item.setId( "IRON_SWORD" );
    catalog.addItem( item );

    EXPECT_EQ( catalog.item( "iron_sword" ), nullptr );
}

TEST( ItemCatalogTest, AddItem_DuplicateId_KeepsFirstEntry ) {
    Engine::ItemCatalog catalog;

    Engine::ItemModel first;
    first.setId( "IRON_SWORD" );
    first.setName( "Iron Sword" );
    catalog.addItem( first );

    Engine::ItemModel second;
    second.setId( "IRON_SWORD" );
    second.setName( "Steel Sword" );
    catalog.addItem( second );

    EXPECT_EQ( catalog.item( "IRON_SWORD" )->name(), "Iron Sword" );
}

TEST( ItemCatalogTest, Items_ReturnsAllAddedEntries ) {
    Engine::ItemCatalog catalog;

    Engine::ItemModel first;
    first.setId( "IRON_SWORD" );
    catalog.addItem( first );

    Engine::ItemModel second;
    second.setId( "WOODEN_SHIELD" );
    catalog.addItem( second );

    EXPECT_EQ( catalog.items().size(), 2u );
}
