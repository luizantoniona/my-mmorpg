#include <gtest/gtest.h>

#include <filesystem>
#include <string>

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

class WrongTypeCase {
public:
    WrongTypeCase( std::string name, std::string key, Json::Value value ) :
        name( std::move( name ) ),
        key( std::move( key ) ),
        value( std::move( value ) ) {
    }

    std::string name;
    std::string key;
    Json::Value value;
};

class TileFactoryWrongTypeTest : public TileFactoryTest, public ::testing::WithParamInterface<WrongTypeCase> {};

} // namespace

TEST_F( TileFactoryTest, CreateTileCatalog_ParsesFields ) {
    Json::Value tile;
    tile[ "Type" ] = 1;
    tile[ "Name" ] = "Grass";
    tile[ "TextureFile" ] = "Textures/Tiles/grass_01";
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
    EXPECT_EQ( catalog.tile( 1 )->textureFile(), "Textures/Tiles/grass_01" );
    EXPECT_FALSE( catalog.tile( 1 )->isWalkable() );
    ASSERT_EQ( catalog.tile( 1 )->tags().size(), 1 );
    EXPECT_EQ( catalog.tile( 1 )->tags().first(), "nature" );
}

TEST_P( TileFactoryWrongTypeTest, CreateTileCatalog_WrongFieldType_SkipsEntryAndKeepsOthers ) {
    Json::Value invalid;
    invalid[ "Type" ] = 1;
    invalid[ "Name" ] = "Bad";
    invalid[ "TextureFile" ] = "Textures/Tiles/bad";
    invalid[ GetParam().key ] = GetParam().value;

    Json::Value valid;
    valid[ "Type" ] = 2;
    valid[ "Name" ] = "Grass";
    valid[ "TextureFile" ] = "Textures/Tiles/grass_01";

    Json::Value tiles( Json::arrayValue );
    tiles.append( invalid );
    tiles.append( valid );
    writeTilesJson( tiles );

    Engine::TileCatalog catalog;
    Engine::TileFactory::createTileCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.tile( 1 ), nullptr );
    EXPECT_NE( catalog.tile( 2 ), nullptr );
    EXPECT_EQ( catalog.tiles().size(), 1u );
}

INSTANTIATE_TEST_SUITE_P(
    Fields,
    TileFactoryWrongTypeTest,
    ::testing::Values(
        WrongTypeCase( "TypeIsText", "Type", Json::Value( "one" ) ),
        WrongTypeCase( "NameIsNumber", "Name", Json::Value( 5 ) ),
        WrongTypeCase( "TextureFileIsNumber", "TextureFile", Json::Value( 5 ) ),
        WrongTypeCase( "TagsIsText", "Tags", Json::Value( "nature" ) ),
        WrongTypeCase( "IsWalkableIsText", "IsWalkable", Json::Value( "yes" ) ),
        WrongTypeCase( "FrameDurationIsText", "FrameDurationMs", Json::Value( "fast" ) ) ),
    []( const ::testing::TestParamInfo<WrongTypeCase>& info ) { return info.param.name; } );

TEST_F( TileFactoryTest, CreateTileCatalog_NonStringTag_IsIgnoredButTileLoads ) {
    Json::Value tile;
    tile[ "Type" ] = 1;
    tile[ "Name" ] = "Grass";
    tile[ "TextureFile" ] = "Textures/Tiles/grass_01";
    tile[ "Tags" ] = Json::Value( Json::arrayValue );
    tile[ "Tags" ].append( "nature" );
    tile[ "Tags" ].append( 7 );

    Json::Value tiles( Json::arrayValue );
    tiles.append( tile );
    writeTilesJson( tiles );

    Engine::TileCatalog catalog;
    Engine::TileFactory::createTileCatalog( configPath(), catalog );

    ASSERT_NE( catalog.tile( 1 ), nullptr );
    EXPECT_EQ( catalog.tile( 1 )->tags(), QStringList( { "nature" } ) );
}

TEST_F( TileFactoryTest, CreateTileCatalog_EntryNotAnObject_IsSkipped ) {
    Json::Value valid;
    valid[ "Type" ] = 2;
    valid[ "Name" ] = "Grass";
    valid[ "TextureFile" ] = "Textures/Tiles/grass_01";

    Json::Value tiles( Json::arrayValue );
    tiles.append( 5 );
    tiles.append( valid );
    writeTilesJson( tiles );

    Engine::TileCatalog catalog;
    Engine::TileFactory::createTileCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.tiles().size(), 1u );
    EXPECT_NE( catalog.tile( 2 ), nullptr );
}

TEST_F( TileFactoryTest, CreateTileCatalog_MissingIsWalkable_DefaultsToTrue ) {
    Json::Value tile;
    tile[ "Type" ] = 1;
    tile[ "Name" ] = "Grass";
    tile[ "TextureFile" ] = "Textures/Tiles/grass_01";

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
    first[ "TextureFile" ] = "Textures/Tiles/grass_01";

    Json::Value second;
    second[ "Type" ] = 2;
    second[ "Name" ] = "Sand";
    second[ "TextureFile" ] = "Textures/Tiles/sand_01";

    Json::Value tiles( Json::arrayValue );
    tiles.append( first );
    tiles.append( second );
    writeTilesJson( tiles );

    Engine::TileCatalog catalog;
    Engine::TileFactory::createTileCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.tiles().size(), 2u );
}

TEST_F( TileFactoryTest, CreateTileCatalog_SameNameDifferentTextureFile_BothKept ) {
    Json::Value first;
    first[ "Type" ] = 1;
    first[ "Name" ] = "Stone ground";
    first[ "TextureFile" ] = "Textures/Tiles/stone_01";

    Json::Value second;
    second[ "Type" ] = 2;
    second[ "Name" ] = "Stone ground";
    second[ "TextureFile" ] = "Textures/Tiles/stone_02";

    Json::Value tiles( Json::arrayValue );
    tiles.append( first );
    tiles.append( second );
    writeTilesJson( tiles );

    Engine::TileCatalog catalog;
    Engine::TileFactory::createTileCatalog( configPath(), catalog );

    ASSERT_NE( catalog.tile( 1 ), nullptr );
    ASSERT_NE( catalog.tile( 2 ), nullptr );
    EXPECT_EQ( catalog.tile( 1 )->name(), catalog.tile( 2 )->name() );
    EXPECT_NE( catalog.tile( 1 )->textureFile(), catalog.tile( 2 )->textureFile() );
}

TEST_F( TileFactoryTest, SaveTileCatalog_ThenCreateTileCatalog_RoundTripsFields ) {
    Engine::TileCatalog original;
    Engine::TileModel tile;
    tile.setType( 4 );
    tile.setName( "Stone" );
    tile.setTextureFile( "Textures/Tiles/stone_01" );
    tile.setIsWalkable( false );
    tile.setTags( { "solid" } );
    original.addTile( tile );

    Engine::TileFactory::saveTileCatalog( configPath(), original );

    Engine::TileCatalog reloaded;
    Engine::TileFactory::createTileCatalog( configPath(), reloaded );

    ASSERT_NE( reloaded.tile( 4 ), nullptr );
    EXPECT_EQ( reloaded.tile( 4 )->name(), "Stone" );
    EXPECT_EQ( reloaded.tile( 4 )->textureFile(), "Textures/Tiles/stone_01" );
    EXPECT_FALSE( reloaded.tile( 4 )->isWalkable() );
    ASSERT_EQ( reloaded.tile( 4 )->tags().size(), 1 );
    EXPECT_EQ( reloaded.tile( 4 )->tags().first(), "solid" );
}
