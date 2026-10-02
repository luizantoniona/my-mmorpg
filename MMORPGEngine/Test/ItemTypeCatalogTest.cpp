#include <gtest/gtest.h>

#include <MMORPGEngine/Data/Item/ItemTypeCatalog.h>

TEST( ItemTypeCatalogTest, AddItemType_ThenGet_ReturnsSameItemType ) {
    Engine::ItemTypeCatalog catalog;

    Engine::ItemTypeModel itemType;
    itemType.setType( "SWORD" );
    itemType.setName( "Sword" );
    catalog.addItemType( itemType );

    ASSERT_NE( catalog.itemType( "SWORD" ), nullptr );
    EXPECT_EQ( catalog.itemType( "SWORD" )->name(), "Sword" );
}

TEST( ItemTypeCatalogTest, ItemType_UnknownType_ReturnsNullptr ) {
    const Engine::ItemTypeCatalog catalog;

    EXPECT_EQ( catalog.itemType( "GHOST" ), nullptr );
}

TEST( ItemTypeCatalogTest, AddItemType_DuplicateType_KeepsFirstEntry ) {
    Engine::ItemTypeCatalog catalog;

    Engine::ItemTypeModel first;
    first.setType( "SWORD" );
    first.setName( "Sword" );
    catalog.addItemType( first );

    Engine::ItemTypeModel second;
    second.setType( "SWORD" );
    second.setName( "Axe" );
    catalog.addItemType( second );

    EXPECT_EQ( catalog.itemType( "SWORD" )->name(), "Sword" );
}

TEST( ItemTypeCatalogTest, ItemTypes_ReturnsAllAddedEntries ) {
    Engine::ItemTypeCatalog catalog;

    Engine::ItemTypeModel first;
    first.setType( "SWORD" );
    catalog.addItemType( first );

    Engine::ItemTypeModel second;
    second.setType( "AXE" );
    catalog.addItemType( second );

    EXPECT_EQ( catalog.itemTypes().size(), 2u );
}
