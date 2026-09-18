#include <gtest/gtest.h>

#include <MMORPGEngine/Data/Item/ItemTypeCatalog.h>

TEST( ItemTypeCatalogTest, AddItemType_ThenGet_ReturnsSameItemType ) {
    Engine::ItemTypeCatalog catalog;

    Engine::ItemTypeModel itemType;
    itemType.setType( 1 );
    itemType.setName( "Sword" );
    catalog.addItemType( itemType );

    ASSERT_NE( catalog.itemType( 1 ), nullptr );
    EXPECT_EQ( catalog.itemType( 1 )->name(), "Sword" );
}

TEST( ItemTypeCatalogTest, ItemType_UnknownType_ReturnsNullptr ) {
    const Engine::ItemTypeCatalog catalog;

    EXPECT_EQ( catalog.itemType( 99 ), nullptr );
}

TEST( ItemTypeCatalogTest, AddItemType_DuplicateType_KeepsFirstEntry ) {
    Engine::ItemTypeCatalog catalog;

    Engine::ItemTypeModel first;
    first.setType( 1 );
    first.setName( "Sword" );
    catalog.addItemType( first );

    Engine::ItemTypeModel second;
    second.setType( 1 );
    second.setName( "Axe" );
    catalog.addItemType( second );

    EXPECT_EQ( catalog.itemType( 1 )->name(), "Sword" );
}

TEST( ItemTypeCatalogTest, ItemTypes_ReturnsAllAddedEntries ) {
    Engine::ItemTypeCatalog catalog;

    Engine::ItemTypeModel first;
    first.setType( 1 );
    catalog.addItemType( first );

    Engine::ItemTypeModel second;
    second.setType( 2 );
    catalog.addItemType( second );

    EXPECT_EQ( catalog.itemTypes().size(), 2u );
}
