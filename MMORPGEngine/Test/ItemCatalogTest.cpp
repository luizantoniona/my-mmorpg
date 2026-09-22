#include <gtest/gtest.h>

#include <MMORPGEngine/Data/Item/ItemCatalog.h>

TEST( ItemCatalogTest, AddItem_ThenGet_ReturnsSameItem ) {
    Engine::ItemCatalog catalog;

    Engine::ItemModel item;
    item.setId( 1 );
    item.setIdItemType( 1 );
    item.setName( "Iron Sword" );
    catalog.addItem( item );

    ASSERT_NE( catalog.item( 1 ), nullptr );
    EXPECT_EQ( catalog.item( 1 )->name(), "Iron Sword" );
    EXPECT_EQ( catalog.item( 1 )->idItemType(), 1u );
}

TEST( ItemCatalogTest, Item_UnknownId_ReturnsNullptr ) {
    const Engine::ItemCatalog catalog;

    EXPECT_EQ( catalog.item( 99 ), nullptr );
}

TEST( ItemCatalogTest, AddItem_DuplicateId_KeepsFirstEntry ) {
    Engine::ItemCatalog catalog;

    Engine::ItemModel first;
    first.setId( 1 );
    first.setName( "Iron Sword" );
    catalog.addItem( first );

    Engine::ItemModel second;
    second.setId( 1 );
    second.setName( "Steel Sword" );
    catalog.addItem( second );

    EXPECT_EQ( catalog.item( 1 )->name(), "Iron Sword" );
}

TEST( ItemCatalogTest, Items_ReturnsAllAddedEntries ) {
    Engine::ItemCatalog catalog;

    Engine::ItemModel first;
    first.setId( 1 );
    catalog.addItem( first );

    Engine::ItemModel second;
    second.setId( 2 );
    catalog.addItem( second );

    EXPECT_EQ( catalog.items().size(), 2u );
}
