#include <gtest/gtest.h>

#include <MMORPGEngine/Data/Tile/TileCatalog.h>

TEST( TileCatalogTest, AddTile_ThenGet_ReturnsSameTile ) {
    Engine::TileCatalog catalog;

    Engine::TileModel tile;
    tile.setType( 1 );
    tile.setName( "Grass" );
    catalog.addTile( tile );

    ASSERT_NE( catalog.tile( 1 ), nullptr );
    EXPECT_EQ( catalog.tile( 1 )->name(), "Grass" );
}

TEST( TileCatalogTest, Tile_UnknownType_ReturnsNullptr ) {
    const Engine::TileCatalog catalog;

    EXPECT_EQ( catalog.tile( 99 ), nullptr );
}

TEST( TileCatalogTest, AddTile_DuplicateType_KeepsFirstEntry ) {
    Engine::TileCatalog catalog;

    Engine::TileModel first;
    first.setType( 1 );
    first.setName( "Grass" );
    catalog.addTile( first );

    Engine::TileModel second;
    second.setType( 1 );
    second.setName( "Sand" );
    catalog.addTile( second );

    EXPECT_EQ( catalog.tile( 1 )->name(), "Grass" );
}

TEST( TileCatalogTest, Tiles_ReturnsAllAddedEntries ) {
    Engine::TileCatalog catalog;

    Engine::TileModel first;
    first.setType( 1 );
    catalog.addTile( first );

    Engine::TileModel second;
    second.setType( 2 );
    catalog.addTile( second );

    EXPECT_EQ( catalog.tiles().size(), 2u );
}
