#include <gtest/gtest.h>

#include <filesystem>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Tile/TileFactory.h>

namespace {

class TileFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_tilefactory";
        std::filesystem::create_directories( _tempDir / "TestMap" );

        Json::Value configJson;
        configJson[ "ActiveFolder" ] = "TestMap";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "Config.json" ).string(), configJson );

        Json::Value mapJson;
        mapJson[ "Catalogs" ][ "Tiles" ] = "Tile.json";
        mapJson[ "Catalogs" ][ "Objects" ] = "Object.json";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Map.json" ).string(), mapJson );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    void writeTilesJson( const Json::Value& tilesArray ) const {
        Json::Value tileJson;
        tileJson[ "Tiles" ] = tilesArray;
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Tile.json" ).string(), tileJson );
    }

    QString configPath() const {
        return QString::fromStdString( _tempDir.string() ) + "/";
    }

    std::filesystem::path _tempDir;
};

} // namespace

TEST_F( TileFactoryTest, CreateTileCatalog_ParsesFields ) {
    Json::Value tile;
    tile[ "Type" ] = 1;
    tile[ "Name" ] = "Grass";
    tile[ "TextureFolder" ] = "Textures/Tiles/Grass";
    tile[ "IsWalkable" ] = false;
    Json::Value tags( Json::arrayValue );
    tags.append( "nature" );
    tile[ "Tags" ] = tags;

    Json::Value tiles( Json::arrayValue );
    tiles.append( tile );
    writeTilesJson( tiles );

    Engine::TileCatalog catalog;
    Engine::TileFactory::createTileCatalog( configPath(), catalog );

    ASSERT_NE( catalog.tile( 1 ), nullptr );
    EXPECT_EQ( catalog.tile( 1 )->name(), "Grass" );
    EXPECT_EQ( catalog.tile( 1 )->folder(), "Textures/Tiles/Grass" );
    EXPECT_FALSE( catalog.tile( 1 )->isWalkable() );
    ASSERT_EQ( catalog.tile( 1 )->tags().size(), 1 );
    EXPECT_EQ( catalog.tile( 1 )->tags().first(), "nature" );
}

TEST_F( TileFactoryTest, CreateTileCatalog_MissingIsWalkable_DefaultsToTrue ) {
    Json::Value tile;
    tile[ "Type" ] = 1;
    tile[ "Name" ] = "Grass";
    tile[ "TextureFolder" ] = "Textures/Tiles/Grass";

    Json::Value tiles( Json::arrayValue );
    tiles.append( tile );
    writeTilesJson( tiles );

    Engine::TileCatalog catalog;
    Engine::TileFactory::createTileCatalog( configPath(), catalog );

    ASSERT_NE( catalog.tile( 1 ), nullptr );
    EXPECT_TRUE( catalog.tile( 1 )->isWalkable() );
}

TEST_F( TileFactoryTest, CreateTileCatalog_MultipleTiles_AllAdded ) {
    Json::Value first;
    first[ "Type" ] = 1;
    first[ "Name" ] = "Grass";
    first[ "TextureFolder" ] = "Textures/Tiles/Grass";

    Json::Value second;
    second[ "Type" ] = 2;
    second[ "Name" ] = "Sand";
    second[ "TextureFolder" ] = "Textures/Tiles/Sand";

    Json::Value tiles( Json::arrayValue );
    tiles.append( first );
    tiles.append( second );
    writeTilesJson( tiles );

    Engine::TileCatalog catalog;
    Engine::TileFactory::createTileCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.tiles().size(), 2u );
}

TEST_F( TileFactoryTest, SaveTileCatalog_ThenCreateTileCatalog_RoundTripsFields ) {
    Engine::TileCatalog original;
    Engine::TileModel tile;
    tile.setType( 4 );
    tile.setName( "Stone" );
    tile.setFolder( "Textures/Tiles/Stone" );
    tile.setIsWalkable( false );
    tile.setTags( { "solid" } );
    original.addTile( tile );

    Engine::TileFactory::saveTileCatalog( configPath(), original );

    Engine::TileCatalog reloaded;
    Engine::TileFactory::createTileCatalog( configPath(), reloaded );

    ASSERT_NE( reloaded.tile( 4 ), nullptr );
    EXPECT_EQ( reloaded.tile( 4 )->name(), "Stone" );
    EXPECT_EQ( reloaded.tile( 4 )->folder(), "Textures/Tiles/Stone" );
    EXPECT_FALSE( reloaded.tile( 4 )->isWalkable() );
    ASSERT_EQ( reloaded.tile( 4 )->tags().size(), 1 );
    EXPECT_EQ( reloaded.tile( 4 )->tags().first(), "solid" );
}
