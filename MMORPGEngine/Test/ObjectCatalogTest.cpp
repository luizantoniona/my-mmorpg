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

TEST( ObjectCatalogTest, AddObject_DefaultInteraction_IsEmpty ) {
    Engine::ObjectCatalog catalog;

    Engine::ObjectModel object;
    object.setType( 1 );
    catalog.addObject( object );

    EXPECT_FALSE( catalog.object( 1 )->interaction().has_value() );
}

TEST( ObjectCatalogTest, AddObject_ContainerInteraction_KeepsCapacity ) {
    Engine::ObjectCatalog catalog;

    Engine::ObjectModel object;
    object.setType( 1 );

    Engine::ObjectInteractionModel interaction;
    interaction.setType( Engine::ObjectInteractionEnum::CONTAINER );
    interaction.setContainerCapacity( 8 );
    object.setInteraction( interaction );
    catalog.addObject( object );

    ASSERT_TRUE( catalog.object( 1 )->interaction().has_value() );
    EXPECT_EQ( catalog.object( 1 )->interaction()->type(), Engine::ObjectInteractionEnum::CONTAINER );
    EXPECT_EQ( catalog.object( 1 )->interaction()->containerCapacity(), 8u );
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
