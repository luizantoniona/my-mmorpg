#include <gtest/gtest.h>

#include <MMORPGEngine/Data/Object/ObjectCatalog.h>

TEST( ObjectCatalogTest, AddObject_ThenGet_ReturnsSameObject ) {
    Engine::ObjectCatalog catalog;

    Engine::ObjectModel object;
    object.setType( 1 );
    object.setName( "StoneWall" );
    catalog.addObject( object );

    ASSERT_NE( catalog.object( 1 ), nullptr );
    EXPECT_EQ( catalog.object( 1 )->name(), "StoneWall" );
}

TEST( ObjectCatalogTest, Object_UnknownType_ReturnsNullptr ) {
    const Engine::ObjectCatalog catalog;

    EXPECT_EQ( catalog.object( 99 ), nullptr );
}

TEST( ObjectCatalogTest, AddObject_DuplicateType_KeepsFirstEntry ) {
    Engine::ObjectCatalog catalog;

    Engine::ObjectModel first;
    first.setType( 1 );
    first.setName( "StoneWall" );
    catalog.addObject( first );

    Engine::ObjectModel second;
    second.setType( 1 );
    second.setName( "WoodenCrate" );
    catalog.addObject( second );

    EXPECT_EQ( catalog.object( 1 )->name(), "StoneWall" );
}

TEST( ObjectCatalogTest, Objects_ReturnsAllAddedEntries ) {
    Engine::ObjectCatalog catalog;

    Engine::ObjectModel first;
    first.setType( 1 );
    catalog.addObject( first );

    Engine::ObjectModel second;
    second.setType( 2 );
    catalog.addObject( second );

    EXPECT_EQ( catalog.objects().size(), 2u );
}
