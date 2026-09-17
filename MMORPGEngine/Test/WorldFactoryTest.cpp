#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <memory>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/World/WorldFactory.h>

namespace {

constexpr uint32_t kTestTileType = 9201;

class WorldFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        Engine::TileModel tile;
        tile.setType( kTestTileType );
        tile.setName( "WorldFactoryTestTile" );
        Engine::Singleton<Engine::DataManager>::instance().addTile( tile );

        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_worldfactory";
        std::filesystem::create_directories( _tempDir / "TestMap" / "Floors" );

        Json::Value configJson;
        configJson[ "ActiveFolder" ] = "TestMap";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "Config.json" ).string(), configJson );

        writeMapJson( 2, 2, true );
        writeFloorJson( "0.json", 0, kTestTileType );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    void writeMapJson( uint32_t width, uint32_t height, bool includeSpawn ) const {
        Json::Value mapJson;
        mapJson[ "Name" ] = "Test World";
        mapJson[ "Width" ] = width;
        mapJson[ "Height" ] = height;
        mapJson[ "Catalogs" ][ "Tiles" ] = "Tile.json";
        mapJson[ "Catalogs" ][ "Objects" ] = "Object.json";
        Json::Value floors( Json::arrayValue );
        floors.append( "0.json" );
        mapJson[ "Floors" ] = floors;

        if ( includeSpawn ) {
            mapJson[ "SpawnX" ] = 1;
            mapJson[ "SpawnY" ] = 1;
            mapJson[ "SpawnZ" ] = 0;
        }

        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Map.json" ).string(), mapJson );
    }

    void writeFloorJson( const std::string& fileName, int z, uint32_t tileType ) const {
        Json::Value floorJson;
        floorJson[ "Z" ] = z;

        Json::Value row( Json::arrayValue );
        row.append( tileType );
        row.append( tileType );
        Json::Value tiles( Json::arrayValue );
        tiles.append( row );
        tiles.append( row );
        floorJson[ "Tiles" ] = tiles;

        Json::Value emptyRow( Json::arrayValue );
        emptyRow.append( 0 );
        emptyRow.append( 0 );
        Json::Value objects( Json::arrayValue );
        objects.append( emptyRow );
        objects.append( emptyRow );
        floorJson[ "Objects" ] = objects;

        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Floors" / fileName ).string(), floorJson );
    }

    Json::Value readMapJson() const {
        return Engine::JsonHelper::loadJsonFile( ( _tempDir / "TestMap" / "Map.json" ).string() );
    }

    std::string worldPath() const {
        return _tempDir.string() + "/";
    }

    std::filesystem::path _tempDir;
};

} // namespace

TEST_F( WorldFactoryTest, CreateWorld_LoadsNameSizeSpawnAndFloors ) {
    const std::unique_ptr<Engine::WorldModel> world = Engine::WorldFactory::createWorld( worldPath() );

    EXPECT_EQ( world->name(), "Test World" );
    EXPECT_EQ( world->width(), 2u );
    EXPECT_EQ( world->height(), 2u );
    EXPECT_EQ( world->spawnX(), 1 );
    EXPECT_EQ( world->spawnY(), 1 );
    EXPECT_EQ( world->spawnZ(), 0 );

    const std::vector<int> floors = world->floors();
    EXPECT_NE( std::find( floors.begin(), floors.end(), 0 ), floors.end() );

    ASSERT_NE( world->tile( 0, 0, 0 ), nullptr );
    EXPECT_EQ( world->tile( 0, 0, 0 )->tileType(), kTestTileType );
}

TEST_F( WorldFactoryTest, CreateWorld_MissingSpawn_DefaultsToWorldCenter ) {
    writeMapJson( 2, 2, false );

    const std::unique_ptr<Engine::WorldModel> world = Engine::WorldFactory::createWorld( worldPath() );

    EXPECT_EQ( world->spawnX(), 1 );
    EXPECT_EQ( world->spawnY(), 1 );
    EXPECT_EQ( world->spawnZ(), 0 );
}

TEST_F( WorldFactoryTest, SaveWorld_WritesModifiedTileBackToFloorFile ) {
    const std::unique_ptr<Engine::WorldModel> world = Engine::WorldFactory::createWorld( worldPath() );

    auto tile = std::make_unique<Engine::WorldTileModel>();
    tile->setTileType( 424242 );
    world->chunk( 0, 0 )->setTile( 1, 1, 0, std::move( tile ) );

    Engine::WorldFactory::saveWorld( worldPath(), *world );

    const Json::Value savedFloor = Engine::JsonHelper::loadJsonFile( ( _tempDir / "TestMap" / "Floors" / "0.json" ).string() );
    EXPECT_EQ( savedFloor[ "Tiles" ][ 1 ][ 1 ].asUInt(), 424242u );
    EXPECT_EQ( savedFloor[ "Tiles" ][ 0 ][ 0 ].asUInt(), kTestTileType );
}

TEST_F( WorldFactoryTest, AddFloor_Above_CreatesNextFloorFileAndMapEntry ) {
    const std::unique_ptr<Engine::WorldModel> world = Engine::WorldFactory::createWorld( worldPath() );

    ASSERT_TRUE( Engine::WorldFactory::addFloor( worldPath(), *world, true ) );

    EXPECT_TRUE( std::filesystem::exists( _tempDir / "TestMap" / "Floors" / "1.json" ) );

    const Json::Value mapJson = readMapJson();
    bool found = false;
    for ( const Json::Value& floorFile : mapJson[ "Floors" ] ) {
        found = found || floorFile.asString() == "1.json";
    }
    EXPECT_TRUE( found );
}

TEST_F( WorldFactoryTest, AddFloor_Below_CreatesNegativeFloorFile ) {
    const std::unique_ptr<Engine::WorldModel> world = Engine::WorldFactory::createWorld( worldPath() );

    ASSERT_TRUE( Engine::WorldFactory::addFloor( worldPath(), *world, false ) );

    EXPECT_TRUE( std::filesystem::exists( _tempDir / "TestMap" / "Floors" / "-1.json" ) );
}

TEST_F( WorldFactoryTest, RemoveFloor_ExistingExtraFloor_RemovesFileAndMapEntry ) {
    const std::unique_ptr<Engine::WorldModel> world = Engine::WorldFactory::createWorld( worldPath() );
    ASSERT_TRUE( Engine::WorldFactory::addFloor( worldPath(), *world, true ) );

    ASSERT_TRUE( Engine::WorldFactory::removeFloor( worldPath(), 1 ) );

    EXPECT_FALSE( std::filesystem::exists( _tempDir / "TestMap" / "Floors" / "1.json" ) );

    const Json::Value mapJson = readMapJson();
    for ( const Json::Value& floorFile : mapJson[ "Floors" ] ) {
        EXPECT_NE( floorFile.asString(), "1.json" );
    }
}

TEST_F( WorldFactoryTest, RemoveFloor_LastRemainingFloor_ReturnsFalse ) {
    EXPECT_FALSE( Engine::WorldFactory::removeFloor( worldPath(), 0 ) );
    EXPECT_TRUE( std::filesystem::exists( _tempDir / "TestMap" / "Floors" / "0.json" ) );
}

TEST_F( WorldFactoryTest, RemoveFloor_UnknownFloor_ReturnsFalse ) {
    EXPECT_FALSE( Engine::WorldFactory::removeFloor( worldPath(), 99 ) );
}

TEST_F( WorldFactoryTest, ResizeWorld_SmallerSize_ReturnsFalseAndDoesNotModifyMap ) {
    EXPECT_FALSE( Engine::WorldFactory::resizeWorld( worldPath(), 1, 1 ) );

    const Json::Value mapJson = readMapJson();
    EXPECT_EQ( mapJson[ "Width" ].asUInt(), 2u );
    EXPECT_EQ( mapJson[ "Height" ].asUInt(), 2u );
}

TEST_F( WorldFactoryTest, ResizeWorld_LargerSize_UpdatesMapAndGrowsFloorGrid ) {
    ASSERT_TRUE( Engine::WorldFactory::resizeWorld( worldPath(), 4, 4 ) );

    const Json::Value mapJson = readMapJson();
    EXPECT_EQ( mapJson[ "Width" ].asUInt(), 4u );
    EXPECT_EQ( mapJson[ "Height" ].asUInt(), 4u );

    const Json::Value savedFloor = Engine::JsonHelper::loadJsonFile( ( _tempDir / "TestMap" / "Floors" / "0.json" ).string() );
    ASSERT_EQ( savedFloor[ "Tiles" ].size(), 4u );
    ASSERT_EQ( savedFloor[ "Tiles" ][ 0 ].size(), 4u );
    EXPECT_EQ( savedFloor[ "Tiles" ][ 0 ][ 0 ].asUInt(), kTestTileType );
    EXPECT_EQ( savedFloor[ "Tiles" ][ 3 ][ 3 ].asUInt(), 0u );
}
